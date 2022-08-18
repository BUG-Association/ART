#!/usr/bin/env python3

# Single script converter of reradiation matrices from native ART and BFC formats to their corresponding GMM representations.

# Author: Wei Xue <xuewei4d@gmail.com>
# Modified by Thierry Guillemot <thierry.guillemot.work@gmail.com>
# Modified by The ART Development Team
# License: BSD 3 clause

import math
import warnings
import numpy as np

from abc import ABCMeta, abstractmethod
from time import time

from scipy import linalg
from scipy.special import betaln, digamma, gammaln, logsumexp

from sklearn import cluster
from sklearn.base import BaseEstimator, DensityMixin
from sklearn.exceptions import ConvergenceWarning
from sklearn.utils import check_array, check_random_state
from sklearn.utils.extmath import row_norms
from sklearn.utils.validation import check_is_fitted, _check_sample_weight, _deprecate_positional_args

###############################################################################
# Base class for mixture models

def _check_shape(param, param_shape, name):
    """Validate the shape of the input parameter 'param'.

    Parameters
    ----------
    param : array

    param_shape : tuple

    name : string
    """
    param = np.array(param)
    if param.shape != param_shape:
        raise ValueError("The parameter '%s' should have the shape of %s, "
                         "but got %s" % (name, param_shape, param.shape))


def _check_X(X, n_components=None, n_features=None, ensure_min_samples=1):
    """Check the input data X.

    Parameters
    ----------
    X : array-like, shape (n_samples, n_features)

    n_components : int

    Returns
    -------
    X : array, shape (n_samples, n_features)
    """
    X = check_array(X, dtype=[np.float64, np.float32],
                    ensure_min_samples=ensure_min_samples)
    if n_components is not None and X.shape[0] < n_components:
        raise ValueError('Expected n_samples >= n_components '
                         'but got n_components = %d, n_samples = %d'
                         % (n_components, X.shape[0]))
    if n_features is not None and X.shape[1] != n_features:
        raise ValueError("Expected the input data X have %d features, "
                         "but got %d features"
                         % (n_features, X.shape[1]))
    return X


def _check_normalize_sample_weight(sample_weight, X):
    """Set sample_weight if None, and check for correct dtype"""
    if sample_weight is None:
        sample_weight = np.ones(X.shape[0])

    sample_weight_was_none = sample_weight is None

    sample_weight = _check_sample_weight(sample_weight, X, dtype=X.dtype)
    if not sample_weight_was_none:
        # normalize the weights to sum up to n_samples
        # an array of 1 (i.e. samples_weight is None) is already normalized
        n_samples = len(sample_weight)
        scale = n_samples / sample_weight.sum()
        sample_weight *= scale
    return sample_weight


class BaseMixture(DensityMixin, BaseEstimator, metaclass=ABCMeta):
    """Base class for mixture models.

    This abstract class specifies an interface for all mixture classes and
    provides basic common methods for mixture models.
    """

    def __init__(self, n_components, tol, reg_covar,
                 max_iter, n_init, init_params, random_state, warm_start,
                 verbose, verbose_interval):
        self.n_components = n_components
        self.tol = tol
        self.reg_covar = reg_covar
        self.max_iter = max_iter
        self.n_init = n_init
        self.init_params = init_params
        self.random_state = random_state
        self.warm_start = warm_start
        self.verbose = verbose
        self.verbose_interval = verbose_interval

    def _check_initial_parameters(self, X):
        """Check values of the basic parameters.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
        """
        if self.n_components < 1:
            raise ValueError("Invalid value for 'n_components': %d "
                             "Estimation requires at least one component"
                             % self.n_components)

        if self.tol < 0.:
            raise ValueError("Invalid value for 'tol': %.5f "
                             "Tolerance used by the EM must be non-negative"
                             % self.tol)

        if self.n_init < 1:
            raise ValueError("Invalid value for 'n_init': %d "
                             "Estimation requires at least one run"
                             % self.n_init)

        if self.max_iter < 1:
            raise ValueError("Invalid value for 'max_iter': %d "
                             "Estimation requires at least one iteration"
                             % self.max_iter)

        if self.reg_covar < 0.:
            raise ValueError("Invalid value for 'reg_covar': %.5f "
                             "regularization on covariance must be "
                             "non-negative"
                             % self.reg_covar)

        # Check all the parameters values of the derived class
        self._check_parameters(X)

    @abstractmethod
    def _check_parameters(self, X):
        """Check initial parameters of the derived class.

        Parameters
        ----------
        X : array-like, shape  (n_samples, n_features)
        """
        pass

    def _initialize_parameters(self, X, sample_weight, random_state):
        """Initialize the model parameters.

        Parameters
        ----------
        X : array-like, shape  (n_samples, n_features)

        random_state : RandomState
            A random number generator instance that controls the random seed
            used for the method chosen to initialize the parameters.

        sample_weight : array-like, shape (n_samples,), optional
            The weights for each observation in X. If None, all observations
            are assigned equal weight (default: None).
        """
        n_samples, _ = X.shape
        sample_weight_copy = sample_weight.copy()

        if self.init_params == 'kmeans':
            resp = np.zeros((n_samples, self.n_components))
            label = cluster.KMeans(
                n_clusters=self.n_components, n_init=1,
                random_state=random_state).fit(
                    X, sample_weight=sample_weight_copy).labels_
            resp[np.arange(n_samples), label] = 1
        elif self.init_params == 'random':
            resp = random_state.rand(n_samples, self.n_components)
            resp /= resp.sum(axis=1)[:, np.newaxis]
        else:
            raise ValueError("Unimplemented initialization method '%s'"
                             % self.init_params)

        self._initialize(X, sample_weight, resp)

    @abstractmethod
    def _initialize(self, X, sample_weight, resp):
        """Initialize the model parameters of the derived class.

        Parameters
        ----------
        X : array-like, shape  (n_samples, n_features)

        resp : array-like, shape (n_samples, n_components)

        sample_weight : array-like, shape (n_samples,), optional.
        """
        pass

    def fit(self, X, y=None, sample_weight=None):
        """Estimate model parameters with the EM algorithm.

        The method fits the model ``n_init`` times and sets the parameters with
        which the model has the largest likelihood or lower bound. Within each
        trial, the method iterates between E-step and M-step for ``max_iter``
        times until the change of likelihood or lower bound is less than
        ``tol``, otherwise, a ``ConvergenceWarning`` is raised.
        If ``warm_start`` is ``True``, then ``n_init`` is ignored and a single
        initialization is performed upon the first call. Upon consecutive
        calls, training starts where it left off.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
            List of n_features-dimensional data points. Each row
            corresponds to a single data point.

        sample_weight : array-like, shape (n_samples,), optional
            The weights for each observation in X. If None, all observations
            are assigned equal weight (default: None).

        Returns
        -------
        self
        """
        self.fit_predict(X, y, sample_weight)
        return self

    def fit_predict(self, X, y=None, sample_weight=None):
        """Estimate model parameters using X and predict the labels for X.

        The method fits the model n_init times and sets the parameters with
        which the model has the largest likelihood or lower bound. Within each
        trial, the method iterates between E-step and M-step for `max_iter`
        times until the change of likelihood or lower bound is less than
        `tol`, otherwise, a :class:`~sklearn.exceptions.ConvergenceWarning` is
        raised. After fitting, it predicts the most probable label for the
        input data points.

        .. versionadded:: 0.20

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
            List of n_features-dimensional data points. Each row
            corresponds to a single data point.

        y : Ignored
            Not used, present here for API consistency by convention.

        sample_weight : array-like, shape (n_samples,), optional
            The weights for each observation in X. If None, all observations
            are assigned equal weight (default: None).

        Returns
        -------
        labels : array, shape (n_samples,)
            Component labels.
        """
        X = _check_X(X, self.n_components, ensure_min_samples=2)
        self._check_n_features(X, reset=True)
        self._check_initial_parameters(X)

        sample_weight = _check_normalize_sample_weight(sample_weight, X)

        # if we enable warm_start, we will have a unique initialisation
        do_init = not(self.warm_start and hasattr(self, 'converged_'))
        n_init = self.n_init if do_init else 1

        max_lower_bound = -np.infty
        self.converged_ = False

        random_state = check_random_state(self.random_state)

        n_samples, _ = X.shape
        for init in range(n_init):
            self._print_verbose_msg_init_beg(init)

            if do_init:
                self._initialize_parameters(X, sample_weight, random_state)

            lower_bound = (-np.infty if do_init else self.lower_bound_)

            for n_iter in range(1, self.max_iter + 1):
                prev_lower_bound = lower_bound

                log_prob_norm, log_resp = self._e_step(X, sample_weight)
                self._m_step(X, sample_weight, log_resp)
                lower_bound = self._compute_lower_bound(
                    log_resp, log_prob_norm)

                change = lower_bound - prev_lower_bound
                self._print_verbose_msg_iter_end(n_iter, change)

                if abs(change) < self.tol:
                    self.converged_ = True
                    break

            self._print_verbose_msg_init_end(lower_bound)

            if lower_bound > max_lower_bound:
                max_lower_bound = lower_bound
                best_params = self._get_parameters()
                best_n_iter = n_iter

        if not self.converged_:
            warnings.warn('Initialization %d did not converge. '
                          'Try different init parameters, '
                          'or increase max_iter, tol '
                          'or check for degenerate data.'
                          % (init + 1), ConvergenceWarning)

        self._set_parameters(best_params)
        self.n_iter_ = best_n_iter
        self.lower_bound_ = max_lower_bound

        # Always do a final e-step to guarantee that the labels returned by
        # fit_predict(X) are always consistent with fit(X).predict(X)
        # for any value of max_iter and tol (and any random_state).
        _, log_resp = self._e_step(X, sample_weight)

        return log_resp.argmax(axis=1)

    def _e_step(self, X, sample_weight):
        """E step.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        sample_weight : array-like, shape (n_samples,)
            The weights for each observation in X.

        Returns
        -------
        log_prob_norm : float
            Mean of the logarithms of the probabilities of each sample in X

        log_responsibility : array, shape (n_samples, n_components)
            Logarithm of the posterior probabilities (or responsibilities) of
            the point of each sample in X.
        """
        log_prob_norm, log_resp = self._estimate_log_prob_resp(X,
                                                               sample_weight)
        return np.mean(log_prob_norm), log_resp

    @abstractmethod
    def _m_step(self, X, sample_weight, log_resp):
        """M step.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        log_resp : array-like, shape (n_samples, n_components)
            Logarithm of the posterior probabilities (or responsibilities) of
            the point of each sample in X.

        sample_weight : array-like, shape (n_samples,)
            The weights for each observation in X.
        """
        pass

    @abstractmethod
    def _get_parameters(self):
        pass

    @abstractmethod
    def _set_parameters(self, params):
        pass

    def score_samples(self, X, sample_weight=None):
        """Compute the weighted log probabilities for each sample.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
            List of n_features-dimensional data points. Each row
            corresponds to a single data point.

        sample_weight : array-like, shape (n_samples,), optional
            The weights for each observation in X. If None, all observations
            are assigned equal weight (default: None).

        Returns
        -------
        log_prob : array, shape (n_samples,)
            Log probabilities of each data point in X.
        """
        check_is_fitted(self)
        X = _check_X(X, None, self.means_.shape[1])

        sample_weight = _check_normalize_sample_weight(sample_weight, X)

        return sample_weight * logsumexp(
            self._estimate_weighted_log_prob(X, sample_weight), axis=1)

    def score(self, X, y=None, sample_weight=None):
        """Compute the per-sample average log-likelihood of the given data X.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_dimensions)
            List of n_features-dimensional data points. Each row
            corresponds to a single data point.

        sample_weight : array-like, shape (n_samples,), optional
            The weights for each observation in X. If None, all observations
            are assigned equal weight (default: None).

        Returns
        -------
        log_likelihood : float
            Log likelihood of the Gaussian mixture given X.
        """
        return self.score_samples(X, sample_weight).mean()

    def predict(self, X, sample_weight=None):
        """Predict the labels for the data samples in X using trained model.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
            List of n_features-dimensional data points. Each row
            corresponds to a single data point.

        sample_weight : array-like, shape (n_samples,), optional
            The weights for each observation in X. If None, all observations
            are assigned equal weight (default: None).

        Returns
        -------
        labels : array, shape (n_samples,)
            Component labels.
        """
        check_is_fitted(self)
        X = _check_X(X, None, self.means_.shape[1])

        sample_weight = _check_normalize_sample_weight(sample_weight, X)

        return self._estimate_weighted_log_prob(X,
                                                sample_weight).argmax(axis=1)

    def predict_proba(self, X, sample_weight=None):
        """Predict posterior probability of each component given the data.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
            List of n_features-dimensional data points. Each row
            corresponds to a single data point.

        sample_weight : array-like, shape (n_samples,), optional
            The weights for each observation in X. If None, all observations
            are assigned equal weight (default: None).

        Returns
        -------
        resp : array, shape (n_samples, n_components)
            Returns the probability each Gaussian (state) in
            the model given each sample.
        """
        check_is_fitted(self)
        X = _check_X(X, None, self.means_.shape[1])

        sample_weight = _check_normalize_sample_weight(sample_weight, X)

        _, log_resp = self._estimate_log_prob_resp(X, sample_weight)
        return np.exp(log_resp)

    def sample(self, n_samples=1):
        """Generate random samples from the fitted Gaussian distribution.

        Parameters
        ----------
        n_samples : int, optional
            Number of samples to generate. Defaults to 1.

        Returns
        -------
        X : array, shape (n_samples, n_features)
            Randomly generated sample

        y : array, shape (nsamples,)
            Component labels

        """
        check_is_fitted(self)

        if n_samples < 1:
            raise ValueError(
                "Invalid value for 'n_samples': %d . The sampling requires at "
                "least one sample." % (self.n_components))

        _, n_features = self.means_.shape
        rng = check_random_state(self.random_state)
        n_samples_comp = rng.multinomial(n_samples, self.weights_)

        if self.covariance_type == 'full':
            X = np.vstack([
                rng.multivariate_normal(mean, covariance, int(sample))
                for (mean, covariance, sample) in zip(
                    self.means_, self.covariances_, n_samples_comp)])
        elif self.covariance_type == "tied":
            X = np.vstack([
                rng.multivariate_normal(mean, self.covariances_, int(sample))
                for (mean, sample) in zip(
                    self.means_, n_samples_comp)])
        else:
            X = np.vstack([
                mean + rng.randn(sample, n_features) * np.sqrt(covariance)
                for (mean, covariance, sample) in zip(
                    self.means_, self.covariances_, n_samples_comp)])

        y = np.concatenate([np.full(sample, j, dtype=int)
                            for j, sample in enumerate(n_samples_comp)])

        return (X, y)

    def _estimate_weighted_log_prob(self, X, sample_weight):
        """Estimate the weighted log-probabilities, log P(X | Z) + log weights.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        sample_weight : array-like, shape (n_samples,)

        Returns
        -------
        weighted_log_prob : array, shape (n_samples, n_component)
        """
        return (self._estimate_log_prob(X, sample_weight)
                + self._estimate_log_weights())

    @abstractmethod
    def _estimate_log_weights(self):
        """Estimate log-weights in EM algorithm, E[ log pi ] in VB algorithm.

        Returns
        -------
        log_weight : array, shape (n_components, )
        """
        pass

    @abstractmethod
    def _estimate_log_prob(self, X, sample_weight):
        """Estimate the log-probabilities log P(X | Z).

        Compute the log-probabilities per each component for each sample.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        sample_weight : array-like, shape (n_samples,)

        Returns
        -------
        log_prob : array, shape (n_samples, n_component)
        """
        pass

    def _estimate_log_prob_resp(self, X, sample_weight):
        """Estimate log probabilities and responsibilities for each sample.

        Compute the log probabilities, weighted log probabilities per
        component and responsibilities for each sample in X with respect to
        the current state of the model.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        sample_weight : array-like, shape (n_samples,)

        Returns
        -------
        log_prob_norm : array, shape (n_samples,)
            log p(X)

        log_responsibilities : array, shape (n_samples, n_components)
            logarithm of the responsibilities
        """
        weighted_log_prob = self._estimate_weighted_log_prob(X, sample_weight)
        log_prob_norm = logsumexp(weighted_log_prob, axis=1)
        with np.errstate(under='ignore'):
            # ignore underflow
            log_resp = weighted_log_prob - log_prob_norm[:, np.newaxis]
        return log_prob_norm, log_resp

    def _print_verbose_msg_init_beg(self, n_init):
        """Print verbose message on initialization."""
        if self.verbose == 1:
            print("Initialization %d" % n_init)
        elif self.verbose >= 2:
            print("Initialization %d" % n_init)
            self._init_prev_time = time()
            self._iter_prev_time = self._init_prev_time

    def _print_verbose_msg_iter_end(self, n_iter, diff_ll):
        """Print verbose message on initialization."""
        if n_iter % self.verbose_interval == 0:
            if self.verbose == 1:
                print("  Iteration %d" % n_iter)
            elif self.verbose >= 2:
                cur_time = time()
                print("  Iteration %d\t time lapse %.5fs\t ll change %.5f" % (
                    n_iter, cur_time - self._iter_prev_time, diff_ll))
                self._iter_prev_time = cur_time

    def _print_verbose_msg_init_end(self, ll):
        """Print verbose message on the end of iteration."""
        if self.verbose == 1:
            print("Initialization converged: %s" % self.converged_)
        elif self.verbose >= 2:
            print("Initialization converged: %s\t time lapse %.5fs\t ll %.5f" %
                  (self.converged_, time() - self._init_prev_time, ll))


###############################################################################
# Weighted Gaussian Mixture Model

###############################################################################
# Gaussian mixture shape checkers used by the GaussianMixture class

def _check_weights(weights, n_components):
    """Check the user provided 'weights'.

    Parameters
    ----------
    weights : array-like, shape (n_components,)
        The proportions of components of each mixture.

    n_components : int
        Number of components.

    Returns
    -------
    weights : array, shape (n_components,)
    """
    weights = check_array(weights, dtype=[np.float64, np.float32],
                          ensure_2d=False)
    _check_shape(weights, (n_components,), 'weights')

    # check range
    if (any(np.less(weights, 0.)) or
            any(np.greater(weights, 1.))):
        raise ValueError("The parameter 'weights' should be in the range "
                         "[0, 1], but got max value %.5f, min value %.5f"
                         % (np.min(weights), np.max(weights)))

    # check normalization
    if not np.allclose(np.abs(1. - np.sum(weights)), 0.):
        raise ValueError("The parameter 'weights' should be normalized, "
                         "but got sum(weights) = %.5f" % np.sum(weights))
    return weights


def _check_means(means, n_components, n_features):
    """Validate the provided 'means'.

    Parameters
    ----------
    means : array-like, shape (n_components, n_features)
        The centers of the current components.

    n_components : int
        Number of components.

    n_features : int
        Number of features.

    Returns
    -------
    means : array, (n_components, n_features)
    """
    means = check_array(means, dtype=[np.float64, np.float32], ensure_2d=False)
    _check_shape(means, (n_components, n_features), 'means')
    return means


def _check_precision_positivity(precision, covariance_type):
    """Check a precision vector is positive-definite."""
    if np.any(np.less_equal(precision, 0.0)):
        raise ValueError("'%s precision' should be "
                         "positive" % covariance_type)


def _check_precision_matrix(precision, covariance_type):
    """Check a precision matrix is symmetric and positive-definite."""
    if not (np.allclose(precision, precision.T) and
            np.all(linalg.eigvalsh(precision) > 0.)):
        raise ValueError("'%s precision' should be symmetric, "
                         "positive-definite" % covariance_type)


def _check_precisions_full(precisions, covariance_type):
    """Check the precision matrices are symmetric and positive-definite."""
    for prec in precisions:
        _check_precision_matrix(prec, covariance_type)


def _check_precisions(precisions, covariance_type, n_components, n_features):
    """Validate user provided precisions.

    Parameters
    ----------
    precisions : array-like
        'full' : shape of (n_components, n_features, n_features)
        'tied' : shape of (n_features, n_features)
        'diag' : shape of (n_components, n_features)
        'spherical' : shape of (n_components,)

    covariance_type : string

    n_components : int
        Number of components.

    n_features : int
        Number of features.

    Returns
    -------
    precisions : array
    """
    precisions = check_array(precisions, dtype=[np.float64, np.float32],
                             ensure_2d=False,
                             allow_nd=covariance_type == 'full')

    precisions_shape = {'full': (n_components, n_features, n_features),
                        'tied': (n_features, n_features),
                        'diag': (n_components, n_features),
                        'spherical': (n_components,)}
    _check_shape(precisions, precisions_shape[covariance_type],
                 '%s precision' % covariance_type)

    _check_precisions = {'full': _check_precisions_full,
                         'tied': _check_precision_matrix,
                         'diag': _check_precision_positivity,
                         'spherical': _check_precision_positivity}
    _check_precisions[covariance_type](precisions, covariance_type)
    return precisions


###############################################################################
# Gaussian mixture parameters estimators (used by the M-Step)

def _estimate_gaussian_covariances_full(resp, X, sample_weight, nk, means,
                                        reg_covar):
    """Estimate the full covariance matrices.

    Parameters
    ----------
    resp : array-like, shape (n_samples, n_components)

    X : array-like, shape (n_samples, n_features)

    sample_weight : array-like, shape (n_samples,)

    nk : array-like, shape (n_components,)

    means : array-like, shape (n_components, n_features)

    reg_covar : float

    Returns
    -------
    covariances : array, shape (n_components, n_features, n_features)
        The covariance matrix of the current components.
    """
    n_components, n_features = means.shape
    covariances = np.empty((n_components, n_features, n_features))
    for k in range(n_components):
        diff = X - means[k]
        weighted_resp = resp * sample_weight[:, np.newaxis]
        covariances[k] = np.dot(weighted_resp[:, k] * diff.T, diff) / nk[k]
        covariances[k].flat[::n_features + 1] += reg_covar
    return covariances


def _estimate_gaussian_covariances_tied(resp, X, sample_weight, nk, means,
                                        reg_covar):
    """Estimate the tied covariance matrix.

    Parameters
    ----------
    resp : array-like, shape (n_samples, n_components)

    X : array-like, shape (n_samples, n_features)

    sample_weight : array-like, shape (n_samples,)

    nk : array-like, shape (n_components,)

    means : array-like, shape (n_components, n_features)

    reg_covar : float

    Returns
    -------
    covariance : array, shape (n_features, n_features)
        The tied covariance matrix of the components.
    """
    avg_X2 = np.dot(X.T, X * sample_weight[:, np.newaxis])
    avg_means2 = np.dot(nk * means.T, means)
    covariance = avg_X2 - avg_means2
    covariance /= nk.sum()
    covariance.flat[::len(covariance) + 1] += reg_covar
    return covariance


def _estimate_gaussian_covariances_diag(resp, X, sample_weight, nk, means,
                                        reg_covar):
    """Estimate the diagonal covariance vectors.

    Parameters
    ----------
    resp : array-like, shape (n_samples, n_components)

    X : array-like, shape (n_samples, n_features)

    sample_weight : array-like, shape (n_samples,)

    nk : array-like, shape (n_components,)

    means : array-like, shape (n_components, n_features)

    reg_covar : float

    Returns
    -------
    covariances : array, shape (n_components, n_features)
        The covariance vector of the current components.
    """
    weighted_resp = resp * sample_weight[:, np.newaxis]
    avg_X2 = np.dot(weighted_resp.T, X * X) / nk[:, np.newaxis]
    avg_means2 = means ** 2
    avg_X_means = means * np.dot(weighted_resp.T, X) / nk[:, np.newaxis]
    return avg_X2 - 2 * avg_X_means + avg_means2 + reg_covar


def _estimate_gaussian_covariances_spherical(resp, X, sample_weight, nk, means,
                                             reg_covar):
    """Estimate the spherical variance values.

    Parameters
    ----------
    resp : array-like, shape (n_samples, n_components)

    X : array-like, shape (n_samples, n_features)

    sample_weight : array-like, shape (n_samples,)

    nk : array-like, shape (n_components,)

    means : array-like, shape (n_components, n_features)

    reg_covar : float

    Returns
    -------
    variances : array, shape (n_components,)
        The variance values of each components.
    """
    return _estimate_gaussian_covariances_diag(resp, X, sample_weight, nk,
                                               means, reg_covar).mean(1)


def _estimate_gaussian_parameters(X, sample_weight, resp, reg_covar,
                                  covariance_type):
    """Estimate the Gaussian distribution parameters.

    Parameters
    ----------
    X : array-like, shape (n_samples, n_features)
        The input data array.

    sample_weight : array-like, shape (n_samples,)
        The weights for each observation in X.

    resp : array-like, shape (n_samples, n_components)
        The responsibilities for each data sample in X.

    reg_covar : float
        The regularization added to the diagonal of the covariance matrices.

    covariance_type : {'full', 'tied', 'diag', 'spherical'}
        The type of precision matrices.

    Returns
    -------
    nk : array-like, shape (n_components,)
        The numbers of data samples in the current components.

    means : array-like, shape (n_components, n_features)
        The centers of the current components.

    covariances : array-like
        The covariance matrix of the current components.
        The shape depends of the covariance_type.
    """
    nk = ((resp * sample_weight[:, np.newaxis]).sum(axis=0)
          + 10 * np.finfo(resp.dtype).eps)
    means = (np.dot(resp.T, X * sample_weight[:, np.newaxis])
             / nk[:, np.newaxis])
    covariances = {"full": _estimate_gaussian_covariances_full,
                   "tied": _estimate_gaussian_covariances_tied,
                   "diag": _estimate_gaussian_covariances_diag,
                   "spherical": _estimate_gaussian_covariances_spherical
                   }[covariance_type](resp, X, sample_weight, nk, means,
                                      reg_covar)
    return nk, means, covariances


def _compute_precision_cholesky(covariances, covariance_type):
    """Compute the Cholesky decomposition of the precisions.

    Parameters
    ----------
    covariances : array-like
        The covariance matrix of the current components.
        The shape depends of the covariance_type.

    covariance_type : {'full', 'tied', 'diag', 'spherical'}
        The type of precision matrices.

    Returns
    -------
    precisions_cholesky : array-like
        The cholesky decomposition of sample precisions of the current
        components. The shape depends of the covariance_type.
    """
    estimate_precision_error_message = (
        "Fitting the mixture model failed because some components have "
        "ill-defined empirical covariance (for instance caused by singleton "
        "or collapsed samples). Try to decrease the number of components, "
        "or increase reg_covar.")

    if covariance_type == 'full':
        n_components, n_features, _ = covariances.shape
        precisions_chol = np.empty((n_components, n_features, n_features))
        for k, covariance in enumerate(covariances):
            try:
                cov_chol = linalg.cholesky(covariance, lower=True)
            except linalg.LinAlgError:
                raise ValueError(estimate_precision_error_message)
            precisions_chol[k] = linalg.solve_triangular(cov_chol,
                                                         np.eye(n_features),
                                                         lower=True).T
    elif covariance_type == 'tied':
        _, n_features = covariances.shape
        try:
            cov_chol = linalg.cholesky(covariances, lower=True)
        except linalg.LinAlgError:
            raise ValueError(estimate_precision_error_message)
        precisions_chol = linalg.solve_triangular(cov_chol, np.eye(n_features),
                                                  lower=True).T
    else:
        if np.any(np.less_equal(covariances, 0.0)):
            raise ValueError(estimate_precision_error_message)
        precisions_chol = 1. / np.sqrt(covariances)
    return precisions_chol


###############################################################################
# Gaussian mixture probability estimators
def _compute_log_det_cholesky(matrix_chol, covariance_type, n_features):
    """Compute the log-det of the cholesky decomposition of matrices.

    Parameters
    ----------
    matrix_chol : array-like
        Cholesky decompositions of the matrices.
        'full' : shape of (n_components, n_features, n_features)
        'tied' : shape of (n_features, n_features)
        'diag' : shape of (n_components, n_features)
        'spherical' : shape of (n_components,)

    covariance_type : {'full', 'tied', 'diag', 'spherical'}

    n_features : int
        Number of features.

    Returns
    -------
    log_det_precision_chol : array-like, shape (n_components,)
        The determinant of the precision matrix for each component.
    """
    if covariance_type == 'full':
        n_components, _, _ = matrix_chol.shape
        log_det_chol = (np.sum(np.log(
            matrix_chol.reshape(
                n_components, -1)[:, ::n_features + 1]), 1))

    elif covariance_type == 'tied':
        log_det_chol = (np.sum(np.log(np.diag(matrix_chol))))

    elif covariance_type == 'diag':
        log_det_chol = (np.sum(np.log(matrix_chol), axis=1))

    else:
        log_det_chol = n_features * (np.log(matrix_chol))

    return log_det_chol


def _estimate_log_gaussian_prob(X, sample_weight, means, precisions_chol,
                                covariance_type):
    """Estimate the log Gaussian probability.

    Parameters
    ----------
    X : array-like, shape (n_samples, n_features)

    sample_weight : array-like, shape (n_samples,), optional

    means : array-like, shape (n_components, n_features)

    precisions_chol : array-like
        Cholesky decompositions of the precision matrices.
        'full' : shape of (n_components, n_features, n_features)
        'tied' : shape of (n_features, n_features)
        'diag' : shape of (n_components, n_features)
        'spherical' : shape of (n_components,)

    covariance_type : {'full', 'tied', 'diag', 'spherical'}

    Returns
    -------
    log_prob : array, shape (n_samples, n_components)
    """
    n_samples, n_features = X.shape
    n_components, _ = means.shape
    # det(precision_chol) is half of det(precision)
    log_det = _compute_log_det_cholesky(
        precisions_chol, covariance_type, n_features)

    log_det_weighted = - 0.0 * np.log(sample_weight)

    if covariance_type == 'full':
        log_prob = np.empty((n_samples, n_components))
        for k, (mu, prec_chol) in enumerate(zip(means, precisions_chol)):
            y = np.dot(X, prec_chol) - np.dot(mu, prec_chol)
            log_prob[:, k] = np.sum(np.square(y), axis=1)

    elif covariance_type == 'tied':
        log_prob = np.empty((n_samples, n_components))
        for k, mu in enumerate(means):
            y = np.dot(X, precisions_chol) - np.dot(mu, precisions_chol)
            log_prob[:, k] = np.sum(np.square(y), axis=1)

    elif covariance_type == 'diag':
        precisions = precisions_chol ** 2
        log_prob = (np.sum((means ** 2 * precisions), 1) -
                    2. * np.dot(X, (means * precisions).T) +
                    np.dot(X ** 2, precisions.T))

    elif covariance_type == 'spherical':
        precisions = precisions_chol ** 2
        log_prob = (np.sum(means ** 2, 1) * precisions -
                    2 * np.dot(X, means.T * precisions) +
                    np.outer(row_norms(X, squared=True), precisions))
    return (-.5 * (n_features * np.log(2 * np.pi) + log_prob)
            + log_det + log_det_weighted[:, np.newaxis])


class WeightedGaussianMixture(BaseMixture):
    """Gaussian Mixture.

    Representation of a Gaussian mixture model probability distribution.
    This class allows to estimate the parameters of a Gaussian mixture
    distribution.

    Read more in the :ref:`User Guide <gmm>`.

    .. versionadded:: 0.18

    Parameters
    ----------
    n_components : int, defaults to 1.
        The number of mixture components.

    covariance_type : {'full' (default), 'tied', 'diag', 'spherical'}
        String describing the type of covariance parameters to use.
        Must be one of:

        'full'
            each component has its own general covariance matrix
        'tied'
            all components share the same general covariance matrix
        'diag'
            each component has its own diagonal covariance matrix
        'spherical'
            each component has its own single variance

    tol : float, defaults to 1e-3.
        The convergence threshold. EM iterations will stop when the
        lower bound average gain is below this threshold.

    reg_covar : float, defaults to 1e-6.
        Non-negative regularization added to the diagonal of covariance.
        Allows to assure that the covariance matrices are all positive.

    max_iter : int, defaults to 100.
        The number of EM iterations to perform.

    n_init : int, defaults to 1.
        The number of initializations to perform. The best results are kept.

    init_params : {'kmeans', 'random'}, defaults to 'kmeans'.
        The method used to initialize the weights, the means and the
        precisions.
        Must be one of::

            'kmeans' : responsibilities are initialized using kmeans.
            'random' : responsibilities are initialized randomly.

    weights_init : array-like, shape (n_components, ), optional
        The user-provided initial weights, defaults to None.
        If it None, weights are initialized using the `init_params` method.

    means_init : array-like, shape (n_components, n_features), optional
        The user-provided initial means, defaults to None,
        If it None, means are initialized using the `init_params` method.

    precisions_init : array-like, optional.
        The user-provided initial precisions (inverse of the covariance
        matrices), defaults to None.
        If it None, precisions are initialized using the 'init_params' method.
        The shape depends on 'covariance_type'::

            (n_components,)                        if 'spherical',
            (n_features, n_features)               if 'tied',
            (n_components, n_features)             if 'diag',
            (n_components, n_features, n_features) if 'full'

    random_state : int, RandomState instance or None, optional (default=None)
        Controls the random seed given to the method chosen to initialize the
        parameters (see `init_params`).
        In addition, it controls the generation of random samples from the
        fitted distribution (see the method `sample`).
        Pass an int for reproducible output across multiple function calls.
        See :term:`Glossary <random_state>`.

    warm_start : bool, default to False.
        If 'warm_start' is True, the solution of the last fitting is used as
        initialization for the next call of fit(). This can speed up
        convergence when fit is called several times on similar problems.
        In that case, 'n_init' is ignored and only a single initialization
        occurs upon the first call.
        See :term:`the Glossary <warm_start>`.

    verbose : int, default to 0.
        Enable verbose output. If 1 then it prints the current
        initialization and each iteration step. If greater than 1 then
        it prints also the log probability and the time needed
        for each step.

    verbose_interval : int, default to 10.
        Number of iteration done before the next print.

    Attributes
    ----------
    weights_ : array-like, shape (n_components,)
        The weights of each mixture components.

    means_ : array-like, shape (n_components, n_features)
        The mean of each mixture component.

    covariances_ : array-like
        The covariance of each mixture component.
        The shape depends on `covariance_type`::

            (n_components,)                        if 'spherical',
            (n_features, n_features)               if 'tied',
            (n_components, n_features)             if 'diag',
            (n_components, n_features, n_features) if 'full'

    precisions_ : array-like
        The precision matrices for each component in the mixture. A precision
        matrix is the inverse of a covariance matrix. A covariance matrix is
        symmetric positive definite so the mixture of Gaussian can be
        equivalently parameterized by the precision matrices. Storing the
        precision matrices instead of the covariance matrices makes it more
        efficient to compute the log-likelihood of new samples at test time.
        The shape depends on `covariance_type`::

            (n_components,)                        if 'spherical',
            (n_features, n_features)               if 'tied',
            (n_components, n_features)             if 'diag',
            (n_components, n_features, n_features) if 'full'

    precisions_cholesky_ : array-like
        The cholesky decomposition of the precision matrices of each mixture
        component. A precision matrix is the inverse of a covariance matrix.
        A covariance matrix is symmetric positive definite so the mixture of
        Gaussian can be equivalently parameterized by the precision matrices.
        Storing the precision matrices instead of the covariance matrices makes
        it more efficient to compute the log-likelihood of new samples at test
        time. The shape depends on `covariance_type`::

            (n_components,)                        if 'spherical',
            (n_features, n_features)               if 'tied',
            (n_components, n_features)             if 'diag',
            (n_components, n_features, n_features) if 'full'

    converged_ : bool
        True when convergence was reached in fit(), False otherwise.

    n_iter_ : int
        Number of step used by the best fit of EM to reach the convergence.

    lower_bound_ : float
        Lower bound value on the log-likelihood (of the training data with
        respect to the model) of the best fit of EM.

    See Also
    --------
    BayesianGaussianMixture : Gaussian mixture model fit with a variational
        inference.
    """
    @_deprecate_positional_args
    def __init__(self, n_components=1, *, covariance_type='full', tol=1e-3,
                 reg_covar=1e-6, max_iter=100, n_init=1, init_params='kmeans',
                 weights_init=None, means_init=None, precisions_init=None,
                 random_state=None, warm_start=False,
                 verbose=0, verbose_interval=10):
        super().__init__(
            n_components=n_components, tol=tol, reg_covar=reg_covar,
            max_iter=max_iter, n_init=n_init, init_params=init_params,
            random_state=random_state, warm_start=warm_start,
            verbose=verbose, verbose_interval=verbose_interval)

        self.covariance_type = covariance_type
        self.weights_init = weights_init
        self.means_init = means_init
        self.precisions_init = precisions_init

    def _check_parameters(self, X):
        """Check the Gaussian mixture parameters are well defined."""
        _, n_features = X.shape
        if self.covariance_type not in ['spherical', 'tied', 'diag', 'full']:
            raise ValueError("Invalid value for 'covariance_type': %s "
                             "'covariance_type' should be in "
                             "['spherical', 'tied', 'diag', 'full']"
                             % self.covariance_type)

        if self.weights_init is not None:
            self.weights_init = _check_weights(self.weights_init,
                                               self.n_components)

        if self.means_init is not None:
            self.means_init = _check_means(self.means_init,
                                           self.n_components, n_features)

        if self.precisions_init is not None:
            self.precisions_init = _check_precisions(self.precisions_init,
                                                     self.covariance_type,
                                                     self.n_components,
                                                     n_features)

    def _initialize(self, X, sample_weight, resp):
        """Initialization of the Gaussian mixture parameters.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        sample_weight : array-like, shape (n_samples,)

        resp : array-like, shape (n_samples, n_components)
        """
        sum_weight = np.sum(sample_weight)

        weights, means, covariances = _estimate_gaussian_parameters(
            X, sample_weight, resp, self.reg_covar, self.covariance_type)
        weights /= sum_weight

        self.weights_ = (weights if self.weights_init is None
                         else self.weights_init)
        self.means_ = means if self.means_init is None else self.means_init

        if self.precisions_init is None:
            self.covariances_ = covariances
            self.precisions_cholesky_ = _compute_precision_cholesky(
                covariances, self.covariance_type)
        elif self.covariance_type == 'full':
            self.precisions_cholesky_ = np.array(
                [linalg.cholesky(prec_init, lower=True)
                 for prec_init in self.precisions_init])
        elif self.covariance_type == 'tied':
            self.precisions_cholesky_ = linalg.cholesky(self.precisions_init,
                                                        lower=True)
        else:
            self.precisions_cholesky_ = self.precisions_init

    def _m_step(self, X, sample_weight, log_resp):
        """M step.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        sample_weight : array-like, shape (n_samples,)

        log_resp : array-like, shape (n_samples, n_components)
            Logarithm of the posterior probabilities (or responsibilities) of
            the point of each sample in X.
        """
        n_samples, _ = X.shape
        sum_weight = np.sum(sample_weight)
        self.weights_, self.means_, self.covariances_ = (
            _estimate_gaussian_parameters(X, sample_weight, np.exp(log_resp),
                                          self.reg_covar,
                                          self.covariance_type))
        self.weights_ /= sum_weight
        self.precisions_cholesky_ = _compute_precision_cholesky(
            self.covariances_, self.covariance_type)

    def _estimate_log_prob(self, X, sample_weight):
        return _estimate_log_gaussian_prob(X, sample_weight, self.means_,
                                           self.precisions_cholesky_,
                                           self.covariance_type)

    def _estimate_log_weights(self):
        return np.log(self.weights_)

    def _compute_lower_bound(self, _, log_prob_norm):
        return log_prob_norm

    def _get_parameters(self):
        return (self.weights_, self.means_, self.covariances_,
                self.precisions_cholesky_)

    def _set_parameters(self, params):
        (self.weights_, self.means_, self.covariances_,
         self.precisions_cholesky_) = params

        # Attributes computation
        _, n_features = self.means_.shape

        if self.covariance_type == 'full':
            self.precisions_ = np.empty(self.precisions_cholesky_.shape)
            for k, prec_chol in enumerate(self.precisions_cholesky_):
                self.precisions_[k] = np.dot(prec_chol, prec_chol.T)

        elif self.covariance_type == 'tied':
            self.precisions_ = np.dot(self.precisions_cholesky_,
                                      self.precisions_cholesky_.T)
        else:
            self.precisions_ = self.precisions_cholesky_ ** 2

    def _n_parameters(self):
        """Return the number of free parameters in the model."""
        _, n_features = self.means_.shape
        if self.covariance_type == 'full':
            cov_params = self.n_components * n_features * (n_features + 1) / 2.
        elif self.covariance_type == 'diag':
            cov_params = self.n_components * n_features
        elif self.covariance_type == 'tied':
            cov_params = n_features * (n_features + 1) / 2.
        elif self.covariance_type == 'spherical':
            cov_params = self.n_components
        mean_params = n_features * self.n_components
        return int(cov_params + mean_params + self.n_components - 1)

    def bic(self, X, sample_weight=None):
        """Bayesian information criterion for the current model on the input X.

        Parameters
        ----------
        X : array of shape (n_samples, n_dimensions)

        sample_weight : array-like, shape (n_samples,)

        Returns
        -------
        bic : float
            The lower the better.
        """
        sample_weight = _check_normalize_sample_weight(sample_weight, X)
        return (-2 * self.score(X, sample_weight=sample_weight) * X.shape[0]
                + self._n_parameters() * np.log(np.sum(sample_weight)))

    def aic(self, X, sample_weight=None):
        """Akaike information criterion for the current model on the input X.

        Parameters
        ----------
        X : array of shape (n_samples, n_dimensions)

        sample_weight : array-like, shape (n_samples,)

        Returns
        -------
        aic : float
            The lower the better.
        """
        sample_weight = _check_normalize_sample_weight(sample_weight, X)
        return (-2 * self.score(X, sample_weight=sample_weight) * X.shape[0]
                + 2 * self._n_parameters())

###############################################################################
# Weighted Bayesian Gaussian Mixture Model

def _log_dirichlet_norm(dirichlet_concentration):
    """Compute the log of the Dirichlet distribution normalization term.

    Parameters
    ----------
    dirichlet_concentration : array-like, shape (n_samples,)
        The parameters values of the Dirichlet distribution.

    Returns
    -------
    log_dirichlet_norm : float
        The log normalization of the Dirichlet distribution.
    """
    return (gammaln(np.sum(dirichlet_concentration)) -
            np.sum(gammaln(dirichlet_concentration)))


def _log_wishart_norm(degrees_of_freedom, log_det_precisions_chol, n_features):
    """Compute the log of the Wishart distribution normalization term.

    Parameters
    ----------
    degrees_of_freedom : array-like, shape (n_components,)
        The number of degrees of freedom on the covariance Wishart
        distributions.

    log_det_precision_chol : array-like, shape (n_components,)
         The determinant of the precision matrix for each component.

    n_features : int
        The number of features.

    Return
    ------
    log_wishart_norm : array-like, shape (n_components,)
        The log normalization of the Wishart distribution.
    """
    # To simplify the computation we have removed the np.log(np.pi) term
    return -(degrees_of_freedom * log_det_precisions_chol +
             degrees_of_freedom * n_features * .5 * math.log(2.) +
             np.sum(gammaln(.5 * (degrees_of_freedom -
                                  np.arange(n_features)[:, np.newaxis])), 0))


class WeightedBayesianGaussianMixture(BaseMixture):
    """Variational Bayesian estimation of a Gaussian mixture.

    This class allows to infer an approximate posterior distribution over the
    parameters of a Gaussian mixture distribution. The effective number of
    components can be inferred from the data.

    This class implements two types of prior for the weights distribution: a
    finite mixture model with Dirichlet distribution and an infinite mixture
    model with the Dirichlet Process. In practice Dirichlet Process inference
    algorithm is approximated and uses a truncated distribution with a fixed
    maximum number of components (called the Stick-breaking representation).
    The number of components actually used almost always depends on the data.

    .. versionadded:: 0.18

    Read more in the :ref:`User Guide <bgmm>`.

    Parameters
    ----------
    n_components : int, defaults to 1.
        The number of mixture components. Depending on the data and the value
        of the `weight_concentration_prior` the model can decide to not use
        all the components by setting some component `weights_` to values very
        close to zero. The number of effective components is therefore smaller
        than n_components.

    covariance_type : {'full', 'tied', 'diag', 'spherical'}, defaults to 'full'
        String describing the type of covariance parameters to use.
        Must be one of::

            'full' (each component has its own general covariance matrix),
            'tied' (all components share the same general covariance matrix),
            'diag' (each component has its own diagonal covariance matrix),
            'spherical' (each component has its own single variance).

    tol : float, defaults to 1e-3.
        The convergence threshold. EM iterations will stop when the
        lower bound average gain on the likelihood (of the training data with
        respect to the model) is below this threshold.

    reg_covar : float, defaults to 1e-6.
        Non-negative regularization added to the diagonal of covariance.
        Allows to assure that the covariance matrices are all positive.

    max_iter : int, defaults to 100.
        The number of EM iterations to perform.

    n_init : int, defaults to 1.
        The number of initializations to perform. The result with the highest
        lower bound value on the likelihood is kept.

    init_params : {'kmeans', 'random'}, defaults to 'kmeans'.
        The method used to initialize the weights, the means and the
        covariances.
        Must be one of::

            'kmeans' : responsibilities are initialized using kmeans.
            'random' : responsibilities are initialized randomly.

    weight_concentration_prior_type : str, defaults to 'dirichlet_process'.
        String describing the type of the weight concentration prior.
        Must be one of::

            'dirichlet_process' (using the Stick-breaking representation),
            'dirichlet_distribution' (can favor more uniform weights).

    weight_concentration_prior : float | None, optional.
        The dirichlet concentration of each component on the weight
        distribution (Dirichlet). This is commonly called gamma in the
        literature. The higher concentration puts more mass in
        the center and will lead to more components being active, while a lower
        concentration parameter will lead to more mass at the edge of the
        mixture weights simplex. The value of the parameter must be greater
        than 0. If it is None, it's set to ``1. / n_components``.

    mean_precision_prior : float | None, optional.
        The precision prior on the mean distribution (Gaussian).
        Controls the extent of where means can be placed. Larger
        values concentrate the cluster means around `mean_prior`.
        The value of the parameter must be greater than 0.
        If it is None, it is set to 1.

    mean_prior : array-like, shape (n_features,), optional
        The prior on the mean distribution (Gaussian).
        If it is None, it is set to the mean of X.

    degrees_of_freedom_prior : float | None, optional.
        The prior of the number of degrees of freedom on the covariance
        distributions (Wishart). If it is None, it's set to `n_features`.

    covariance_prior : float or array-like, optional
        The prior on the covariance distribution (Wishart).
        If it is None, the emiprical covariance prior is initialized using the
        covariance of X. The shape depends on `covariance_type`::

                (n_features, n_features) if 'full',
                (n_features, n_features) if 'tied',
                (n_features)             if 'diag',
                float                    if 'spherical'

    random_state : int, RandomState instance or None, optional (default=None)
        Controls the random seed given to the method chosen to initialize the
        parameters (see `init_params`).
        In addition, it controls the generation of random samples from the
        fitted distribution (see the method `sample`).
        Pass an int for reproducible output across multiple function calls.
        See :term:`Glossary <random_state>`.

    warm_start : bool, default to False.
        If 'warm_start' is True, the solution of the last fitting is used as
        initialization for the next call of fit(). This can speed up
        convergence when fit is called several times on similar problems.
        See :term:`the Glossary <warm_start>`.

    verbose : int, default to 0.
        Enable verbose output. If 1 then it prints the current
        initialization and each iteration step. If greater than 1 then
        it prints also the log probability and the time needed
        for each step.

    verbose_interval : int, default to 10.
        Number of iteration done before the next print.

    Attributes
    ----------
    weights_ : array-like, shape (n_components,)
        The weights of each mixture components.

    means_ : array-like, shape (n_components, n_features)
        The mean of each mixture component.

    covariances_ : array-like
        The covariance of each mixture component.
        The shape depends on `covariance_type`::

            (n_components,)                        if 'spherical',
            (n_features, n_features)               if 'tied',
            (n_components, n_features)             if 'diag',
            (n_components, n_features, n_features) if 'full'

    precisions_ : array-like
        The precision matrices for each component in the mixture. A precision
        matrix is the inverse of a covariance matrix. A covariance matrix is
        symmetric positive definite so the mixture of Gaussian can be
        equivalently parameterized by the precision matrices. Storing the
        precision matrices instead of the covariance matrices makes it more
        efficient to compute the log-likelihood of new samples at test time.
        The shape depends on ``covariance_type``::

            (n_components,)                        if 'spherical',
            (n_features, n_features)               if 'tied',
            (n_components, n_features)             if 'diag',
            (n_components, n_features, n_features) if 'full'

    precisions_cholesky_ : array-like
        The cholesky decomposition of the precision matrices of each mixture
        component. A precision matrix is the inverse of a covariance matrix.
        A covariance matrix is symmetric positive definite so the mixture of
        Gaussian can be equivalently parameterized by the precision matrices.
        Storing the precision matrices instead of the covariance matrices makes
        it more efficient to compute the log-likelihood of new samples at test
        time. The shape depends on ``covariance_type``::

            (n_components,)                        if 'spherical',
            (n_features, n_features)               if 'tied',
            (n_components, n_features)             if 'diag',
            (n_components, n_features, n_features) if 'full'

    converged_ : bool
        True when convergence was reached in fit(), False otherwise.

    n_iter_ : int
        Number of step used by the best fit of inference to reach the
        convergence.

    lower_bound_ : float
        Lower bound value on the likelihood (of the training data with
        respect to the model) of the best fit of inference.

    weight_concentration_prior_ : tuple or float
        The dirichlet concentration of each component on the weight
        distribution (Dirichlet). The type depends on
        ``weight_concentration_prior_type``::

            (float, float) if 'dirichlet_process' (Beta parameters),
            float          if 'dirichlet_distribution' (Dirichlet parameters).

        The higher concentration puts more mass in
        the center and will lead to more components being active, while a lower
        concentration parameter will lead to more mass at the edge of the
        simplex.

    weight_concentration_ : array-like, shape (n_components,)
        The dirichlet concentration of each component on the weight
        distribution (Dirichlet).

    mean_precision_prior_ : float
        The precision prior on the mean distribution (Gaussian).
        Controls the extent of where means can be placed.
        Larger values concentrate the cluster means around `mean_prior`.
        If mean_precision_prior is set to None, `mean_precision_prior_` is set
        to 1.

    mean_precision_ : array-like, shape (n_components,)
        The precision of each components on the mean distribution (Gaussian).

    mean_prior_ : array-like, shape (n_features,)
        The prior on the mean distribution (Gaussian).

    degrees_of_freedom_prior_ : float
        The prior of the number of degrees of freedom on the covariance
        distributions (Wishart).

    degrees_of_freedom_ : array-like, shape (n_components,)
        The number of degrees of freedom of each components in the model.

    covariance_prior_ : float or array-like
        The prior on the covariance distribution (Wishart).
        The shape depends on `covariance_type`::

            (n_features, n_features) if 'full',
            (n_features, n_features) if 'tied',
            (n_features)             if 'diag',
            float                    if 'spherical'

    See Also
    --------
    GaussianMixture : Finite Gaussian mixture fit with EM.

    References
    ----------

    .. [1] `Bishop, Christopher M. (2006). "Pattern recognition and machine
       learning". Vol. 4 No. 4. New York: Springer.
       <https://www.springer.com/kr/book/9780387310732>`_

    .. [2] `Hagai Attias. (2000). "A Variational Bayesian Framework for
       Graphical Models". In Advances in Neural Information Processing
       Systems 12.
       <http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.36.2841&rep=rep1&type=pdf>`_

    .. [3] `Blei, David M. and Michael I. Jordan. (2006). "Variational
       inference for Dirichlet process mixtures". Bayesian analysis 1.1
       <https://www.cs.princeton.edu/courses/archive/fall11/cos597C/reading/BleiJordan2005.pdf>`_
    """
    @_deprecate_positional_args
    def __init__(self, *, n_components=1, covariance_type='full', tol=1e-3,
                 reg_covar=1e-6, max_iter=100, n_init=1, init_params='kmeans',
                 weight_concentration_prior_type='dirichlet_process',
                 weight_concentration_prior=None,
                 mean_precision_prior=None, mean_prior=None,
                 degrees_of_freedom_prior=None, covariance_prior=None,
                 random_state=None, warm_start=False, verbose=0,
                 verbose_interval=10):
        super().__init__(
            n_components=n_components, tol=tol, reg_covar=reg_covar,
            max_iter=max_iter, n_init=n_init, init_params=init_params,
            random_state=random_state, warm_start=warm_start,
            verbose=verbose, verbose_interval=verbose_interval)

        self.covariance_type = covariance_type
        self.weight_concentration_prior_type = weight_concentration_prior_type
        self.weight_concentration_prior = weight_concentration_prior
        self.mean_precision_prior = mean_precision_prior
        self.mean_prior = mean_prior
        self.degrees_of_freedom_prior = degrees_of_freedom_prior
        self.covariance_prior = covariance_prior

    def _check_parameters(self, X):
        """Check that the parameters are well defined.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
        """
        if self.covariance_type not in ['spherical', 'tied', 'diag', 'full']:
            raise ValueError("Invalid value for 'covariance_type': %s "
                             "'covariance_type' should be in "
                             "['spherical', 'tied', 'diag', 'full']"
                             % self.covariance_type)

        if (self.weight_concentration_prior_type not in
                ['dirichlet_process', 'dirichlet_distribution']):
            raise ValueError(
                "Invalid value for 'weight_concentration_prior_type': %s "
                "'weight_concentration_prior_type' should be in "
                "['dirichlet_process', 'dirichlet_distribution']"
                % self.weight_concentration_prior_type)

        self._check_weights_parameters()
        self._check_means_parameters(X)
        self._check_precision_parameters(X)
        self._checkcovariance_prior_parameter(X)

    def _check_weights_parameters(self):
        """Check the parameter of the Dirichlet distribution."""
        if self.weight_concentration_prior is None:
            self.weight_concentration_prior_ = 1. / self.n_components
        elif self.weight_concentration_prior > 0.:
            self.weight_concentration_prior_ = (
                self.weight_concentration_prior)
        else:
            raise ValueError("The parameter 'weight_concentration_prior' "
                             "should be greater than 0., but got %.3f."
                             % self.weight_concentration_prior)

    def _check_means_parameters(self, X):
        """Check the parameters of the Gaussian distribution.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
        """
        _, n_features = X.shape

        if self.mean_precision_prior is None:
            self.mean_precision_prior_ = 1.
        elif self.mean_precision_prior > 0.:
            self.mean_precision_prior_ = self.mean_precision_prior
        else:
            raise ValueError("The parameter 'mean_precision_prior' should be "
                             "greater than 0., but got %.3f."
                             % self.mean_precision_prior)

        if self.mean_prior is None:
            self.mean_prior_ = X.mean(axis=0)
        else:
            self.mean_prior_ = check_array(self.mean_prior,
                                           dtype=[np.float64, np.float32],
                                           ensure_2d=False)
            _check_shape(self.mean_prior_, (n_features, ), 'means')

    def _check_precision_parameters(self, X):
        """Check the prior parameters of the precision distribution.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
        """
        _, n_features = X.shape

        if self.degrees_of_freedom_prior is None:
            self.degrees_of_freedom_prior_ = n_features
        elif self.degrees_of_freedom_prior > n_features - 1.:
            self.degrees_of_freedom_prior_ = self.degrees_of_freedom_prior
        else:
            raise ValueError("The parameter 'degrees_of_freedom_prior' "
                             "should be greater than %d, but got %.3f."
                             % (n_features - 1, self.degrees_of_freedom_prior))

    def _checkcovariance_prior_parameter(self, X):
        """Check the `covariance_prior_`.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)
        """
        _, n_features = X.shape

        if self.covariance_prior is None:
            self.covariance_prior_ = {
                'full': np.atleast_2d(np.cov(X.T)),
                'tied': np.atleast_2d(np.cov(X.T)),
                'diag': np.var(X, axis=0, ddof=1),
                'spherical': np.var(X, axis=0, ddof=1).mean()
            }[self.covariance_type]

        elif self.covariance_type in ['full', 'tied']:
            self.covariance_prior_ = check_array(
                self.covariance_prior, dtype=[np.float64, np.float32],
                ensure_2d=False)
            _check_shape(self.covariance_prior_, (n_features, n_features),
                         '%s covariance_prior' % self.covariance_type)
            _check_precision_matrix(self.covariance_prior_,
                                    self.covariance_type)
        elif self.covariance_type == 'diag':
            self.covariance_prior_ = check_array(
                self.covariance_prior, dtype=[np.float64, np.float32],
                ensure_2d=False)
            _check_shape(self.covariance_prior_, (n_features,),
                         '%s covariance_prior' % self.covariance_type)
            _check_precision_positivity(self.covariance_prior_,
                                        self.covariance_type)
        # spherical case
        elif self.covariance_prior > 0.:
            self.covariance_prior_ = self.covariance_prior
        else:
            raise ValueError("The parameter 'spherical covariance_prior' "
                             "should be greater than 0., but got %.3f."
                             % self.covariance_prior)

    def _initialize(self, X, sample_weight, resp):
        """Initialization of the mixture parameters.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        resp : array-like, shape (n_samples, n_components)
        """
        nk, xk, sk = _estimate_gaussian_parameters(X, sample_weight, resp,
                                                   self.reg_covar,
                                                   self.covariance_type)

        self._estimate_weights(nk)
        self._estimate_means(nk, xk)
        self._estimate_precisions(nk, xk, sk)

    def _estimate_weights(self, nk):
        """Estimate the parameters of the Dirichlet distribution.

        Parameters
        ----------
        nk : array-like, shape (n_components,)
        """
        if self.weight_concentration_prior_type == 'dirichlet_process':
            # For dirichlet process weight_concentration will be a tuple
            # containing the two parameters of the beta distribution
            self.weight_concentration_ = (
                1. + nk,
                (self.weight_concentration_prior_ +
                 np.hstack((np.cumsum(nk[::-1])[-2::-1], 0))))
        else:
            # case Variationnal Gaussian mixture with dirichlet distribution
            self.weight_concentration_ = self.weight_concentration_prior_ + nk

    def _estimate_means(self, nk, xk):
        """Estimate the parameters of the Gaussian distribution.

        Parameters
        ----------
        nk : array-like, shape (n_components,)

        xk : array-like, shape (n_components, n_features)
        """
        self.mean_precision_ = self.mean_precision_prior_ + nk
        self.means_ = ((self.mean_precision_prior_ * self.mean_prior_ +
                        nk[:, np.newaxis] * xk) /
                       self.mean_precision_[:, np.newaxis])

    def _estimate_precisions(self, nk, xk, sk):
        """Estimate the precisions parameters of the precision distribution.

        Parameters
        ----------
        nk : array-like, shape (n_components,)

        xk : array-like, shape (n_components, n_features)

        sk : array-like
            The shape depends of `covariance_type`:
            'full' : (n_components, n_features, n_features)
            'tied' : (n_features, n_features)
            'diag' : (n_components, n_features)
            'spherical' : (n_components,)
        """
        {"full": self._estimate_wishart_full,
         "tied": self._estimate_wishart_tied,
         "diag": self._estimate_wishart_diag,
         "spherical": self._estimate_wishart_spherical
         }[self.covariance_type](nk, xk, sk)

        self.precisions_cholesky_ = _compute_precision_cholesky(
            self.covariances_, self.covariance_type)

    def _estimate_wishart_full(self, nk, xk, sk):
        """Estimate the full Wishart distribution parameters.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        nk : array-like, shape (n_components,)

        xk : array-like, shape (n_components, n_features)

        sk : array-like, shape (n_components, n_features, n_features)
        """
        _, n_features = xk.shape

        # Warning : in some Bishop book, there is a typo on the formula 10.63
        # `degrees_of_freedom_k = degrees_of_freedom_0 + Nk` is
        # the correct formula
        self.degrees_of_freedom_ = self.degrees_of_freedom_prior_ + nk

        self.covariances_ = np.empty((self.n_components, n_features,
                                      n_features))

        for k in range(self.n_components):
            diff = xk[k] - self.mean_prior_
            self.covariances_[k] = (self.covariance_prior_ + nk[k] * sk[k] +
                                    nk[k] * self.mean_precision_prior_ /
                                    self.mean_precision_[k] * np.outer(diff,
                                                                       diff))

        # Contrary to the original bishop book, we normalize the covariances
        self.covariances_ /= (
            self.degrees_of_freedom_[:, np.newaxis, np.newaxis])

    def _estimate_wishart_tied(self, nk, xk, sk):
        """Estimate the tied Wishart distribution parameters.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        nk : array-like, shape (n_components,)

        xk : array-like, shape (n_components, n_features)

        sk : array-like, shape (n_features, n_features)
        """
        _, n_features = xk.shape

        # Warning : in some Bishop book, there is a typo on the formula 10.63
        # `degrees_of_freedom_k = degrees_of_freedom_0 + Nk`
        # is the correct formula
        self.degrees_of_freedom_ = (
            self.degrees_of_freedom_prior_ + nk.sum() / self.n_components)

        diff = xk - self.mean_prior_
        self.covariances_ = (
            self.covariance_prior_ + sk * nk.sum() / self.n_components +
            self.mean_precision_prior_ / self.n_components * np.dot(
                (nk / self.mean_precision_) * diff.T, diff))

        # Contrary to the original bishop book, we normalize the covariances
        self.covariances_ /= self.degrees_of_freedom_

    def _estimate_wishart_diag(self, nk, xk, sk):
        """Estimate the diag Wishart distribution parameters.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        nk : array-like, shape (n_components,)

        xk : array-like, shape (n_components, n_features)

        sk : array-like, shape (n_components, n_features)
        """
        _, n_features = xk.shape

        # Warning : in some Bishop book, there is a typo on the formula 10.63
        # `degrees_of_freedom_k = degrees_of_freedom_0 + Nk`
        # is the correct formula
        self.degrees_of_freedom_ = self.degrees_of_freedom_prior_ + nk

        diff = xk - self.mean_prior_
        self.covariances_ = (
            self.covariance_prior_ + nk[:, np.newaxis] * (
                sk + (self.mean_precision_prior_ /
                      self.mean_precision_)[:, np.newaxis] * np.square(diff)))

        # Contrary to the original bishop book, we normalize the covariances
        self.covariances_ /= self.degrees_of_freedom_[:, np.newaxis]

    def _estimate_wishart_spherical(self, nk, xk, sk):
        """Estimate the spherical Wishart distribution parameters.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        nk : array-like, shape (n_components,)

        xk : array-like, shape (n_components, n_features)

        sk : array-like, shape (n_components,)
        """
        _, n_features = xk.shape

        # Warning : in some Bishop book, there is a typo on the formula 10.63
        # `degrees_of_freedom_k = degrees_of_freedom_0 + Nk`
        # is the correct formula
        self.degrees_of_freedom_ = self.degrees_of_freedom_prior_ + nk

        diff = xk - self.mean_prior_
        self.covariances_ = (
            self.covariance_prior_ + nk * (
                sk + self.mean_precision_prior_ / self.mean_precision_ *
                np.mean(np.square(diff), 1)))

        # Contrary to the original bishop book, we normalize the covariances
        self.covariances_ /= self.degrees_of_freedom_

    def _m_step(self, X, sample_weight, log_resp):
        """M step.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        log_resp : array-like, shape (n_samples, n_components)
            Logarithm of the posterior probabilities (or responsibilities) of
            the point of each sample in X.
        """
        n_samples, _ = X.shape

        nk, xk, sk = _estimate_gaussian_parameters(
            X, sample_weight, np.exp(log_resp), self.reg_covar,
            self.covariance_type)
        self._estimate_weights(nk)
        self._estimate_means(nk, xk)
        self._estimate_precisions(nk, xk, sk)

    def _estimate_log_weights(self):
        if self.weight_concentration_prior_type == 'dirichlet_process':
            digamma_sum = digamma(self.weight_concentration_[0] +
                                  self.weight_concentration_[1])
            digamma_a = digamma(self.weight_concentration_[0])
            digamma_b = digamma(self.weight_concentration_[1])
            return (digamma_a - digamma_sum +
                    np.hstack((0, np.cumsum(digamma_b - digamma_sum)[:-1])))
        else:
            # case Variationnal Gaussian mixture with dirichlet distribution
            return (digamma(self.weight_concentration_) -
                    digamma(np.sum(self.weight_concentration_)))

    def _estimate_log_prob(self, X, sample_weight):
        _, n_features = X.shape
        # We remove `n_features * np.log(self.degrees_of_freedom_)` because
        # the precision matrix is normalized
        log_gauss = (_estimate_log_gaussian_prob(
            X, sample_weight, self.means_, self.precisions_cholesky_,
            self.covariance_type) -
            .5 * n_features * np.log(self.degrees_of_freedom_))

        log_lambda = n_features * np.log(2.) + np.sum(digamma(
            .5 * (self.degrees_of_freedom_ -
                  np.arange(0, n_features)[:, np.newaxis])), 0)

        return log_gauss + .5 * (log_lambda -
                                 n_features / self.mean_precision_)

    def _compute_lower_bound(self, log_resp, log_prob_norm):
        """Estimate the lower bound of the model.

        The lower bound on the likelihood (of the training data with respect to
        the model) is used to detect the convergence and has to decrease at
        each iteration.

        Parameters
        ----------
        X : array-like, shape (n_samples, n_features)

        log_resp : array, shape (n_samples, n_components)
            Logarithm of the posterior probabilities (or responsibilities) of
            the point of each sample in X.

        log_prob_norm : float
            Logarithm of the probability of each sample in X.

        Returns
        -------
        lower_bound : float
        """
        # Contrary to the original formula, we have done some simplification
        # and removed all the constant terms.
        n_features, = self.mean_prior_.shape

        # We removed `.5 * n_features * np.log(self.degrees_of_freedom_)`
        # because the precision matrix is normalized.
        log_det_precisions_chol = (_compute_log_det_cholesky(
            self.precisions_cholesky_, self.covariance_type, n_features) -
            .5 * n_features * np.log(self.degrees_of_freedom_))

        if self.covariance_type == 'tied':
            log_wishart = self.n_components * np.float64(_log_wishart_norm(
                self.degrees_of_freedom_, log_det_precisions_chol, n_features))
        else:
            log_wishart = np.sum(_log_wishart_norm(
                self.degrees_of_freedom_, log_det_precisions_chol, n_features))

        if self.weight_concentration_prior_type == 'dirichlet_process':
            log_norm_weight = -np.sum(betaln(self.weight_concentration_[0],
                                             self.weight_concentration_[1]))
        else:
            log_norm_weight = _log_dirichlet_norm(self.weight_concentration_)

        return (-np.sum(np.exp(log_resp) * log_resp) -
                log_wishart - log_norm_weight -
                0.5 * n_features * np.sum(np.log(self.mean_precision_)))

    def _get_parameters(self):
        return (self.weight_concentration_,
                self.mean_precision_, self.means_,
                self.degrees_of_freedom_, self.covariances_,
                self.precisions_cholesky_)

    def _set_parameters(self, params):
        (self.weight_concentration_, self.mean_precision_, self.means_,
         self.degrees_of_freedom_, self.covariances_,
         self.precisions_cholesky_) = params

        # Weights computation
        if self.weight_concentration_prior_type == "dirichlet_process":
            weight_dirichlet_sum = (self.weight_concentration_[0] +
                                    self.weight_concentration_[1])
            tmp = self.weight_concentration_[1] / weight_dirichlet_sum
            self.weights_ = (
                self.weight_concentration_[0] / weight_dirichlet_sum *
                np.hstack((1, np.cumprod(tmp[:-1]))))
            self.weights_ /= np.sum(self.weights_)
        else:
            self. weights_ = (self.weight_concentration_ /
                              np.sum(self.weight_concentration_))

        # Precisions matrices computation
        if self.covariance_type == 'full':
            self.precisions_ = np.array([
                np.dot(prec_chol, prec_chol.T)
                for prec_chol in self.precisions_cholesky_])

        elif self.covariance_type == 'tied':
            self.precisions_ = np.dot(self.precisions_cholesky_,
                                      self.precisions_cholesky_.T)
        else:
            self.precisions_ = self.precisions_cholesky_ ** 2

###############################################################################
# Compact Fluorescence - Fluorescent (reradiation) spectrum

import pathlib

class FluoSpectrum:
    wavelength_i_start = 0
    wavelength_i_end = 0
    wavelength_i_sampling = 1
    wavelength_i_n_samples = 0
    
    wavelength_o_start = 0
    wavelength_o_end = 0
    wavelength_o_sampling = 1
    wavelength_o_n_samples = 0

    data = np.zeros((wavelength_i_n_samples, wavelength_o_n_samples, 1))

    def __init__(self, file_path: pathlib.Path):
        tmp_data = []

        with open(file_path, 'r') as f:
            if any(".bfc".casefold() in suffix.casefold() for suffix in file_path.suffixes):
                # Load BFC file

                line_number = 0
                for line in f:
                    line_number += 1

                    end_of_data = line.startswith('EOD') # Last line of BFC

                    if line_number < 11:
                        pass
                    elif line_number == 11:
                        # We parse the sampling frequency and boundaries
                        (self.wavelength_o_start, 
                         self.wavelength_o_end, 
                         self.wavelength_o_sampling,
                         self.wavelength_i_n_samples,
                         self.wavelength_i_start,
                         self.wavelength_i_sampling) = [int(el) for el in line.split()]

                        self.wavelength_o_n_samples = int((self.wavelength_o_end - self.wavelength_o_start) / self.wavelength_o_sampling) + 1
                    elif line_number > 12 and not end_of_data:
                        # Populate the data
                        read_data = [float(el) for el in line.split()[1:]]

                        # This avoids a bug in file having an empty line in the middle of data
                        if (len(read_data) > 0):
                            tmp_data.append(read_data)

                self.data = np.reshape(tmp_data, (self.wavelength_o_n_samples, self.wavelength_i_n_samples))
            else:
                # Try to load ART reradiation file

                header_pos = 0
                for line in f:
                    # Read header info
                    if line.startswith('#'):
                        a, b = line[1:].split()
                        if header_pos == 0:
                            self.wavelength_i_n_samples = int(a)
                            self.wavelength_o_n_samples = int(b)
                        elif header_pos == 1:
                            self.wavelength_i_start = float(a)
                            self.wavelength_i_sampling = float(b)
                        elif header_pos == 2:
                            self.wavelength_o_start = float(a)
                            self.wavelength_o_sampling = float(b)
                        header_pos += 1
                    else:
                        tmp_data.append([float(el) for el in line.split()])

                self.data = np.reshape(tmp_data, (self.wavelength_o_n_samples, self.wavelength_i_n_samples))                
                self.wavelength_o_end = self.wavelength_o_start + (self.wavelength_o_n_samples - 1) * self.wavelength_o_sampling

        self.wavelength_i_end = self.wavelength_i_start + (self.wavelength_i_n_samples - 1) * self.wavelength_i_sampling

        if self.wavelength_i_sampling != self.wavelength_o_sampling:
            raise Exception("Reradiation spectrum with varying incident/outgoing sampling precisions is not supported!")
                

        start_wl = max(self.wavelength_i_start, self.wavelength_o_start)
        start_wl_i_idx = self.idx_for_wl_in(start_wl)
        start_wl_o_idx = self.idx_for_wl_out(start_wl)

        # Surface reflectance (diagonal values) can not be negative
        for idx_i, idx_o in zip(range(start_wl_i_idx, self.wavelength_i_n_samples), 
                                range(start_wl_o_idx, self.wavelength_o_n_samples)):
            if self.data[idx_o, idx_i] < 0:
                self.data[idx_o, idx_i] = 0
                

    # Returns the reradiation matrix without the reflectance data (diagonal)
    def get_pure_fluo(self):
        start_wl = max(self.wavelength_i_start, self.wavelength_o_start)

        start_wl_i_idx = self.idx_for_wl_in(start_wl)
        start_wl_o_idx = self.idx_for_wl_out(start_wl)

        ret_array = self.data.copy()

        # Zeroing out the diagonal
        for idx_i, idx_o in zip(range(start_wl_i_idx, self.wavelength_i_n_samples), 
                                range(start_wl_o_idx, self.wavelength_o_n_samples)):
            ret_array[idx_o, idx_i] = 0

        return ret_array

    # Returns the reradiation matrix without the reflectance data (diagonal)
    # and zeroed negative values and values under the diagonal.
    def get_pure_fluo_filtered(self):
        fluo = self.get_pure_fluo()

        for o in range(fluo.shape[0]):
            for i in range(fluo.shape[1]):
                # Zeroing out invalid negative values
                if fluo[o, i] < 0:
                    fluo[o, i] = 0

                # Zeroing out invalid (nonzero) values under the diagonal
                if self.wl_for_idx_in(i) >= self.wl_for_idx_out(o):
                    fluo[o, i] = 0

        return fluo
    
    # Returns the diagonal of the reradiation matrix
    def get_non_fluo(self):
        start_wl = max(self.wavelength_i_start, self.wavelength_o_start)

        start_wl_i_idx = self.idx_for_wl_in(start_wl)
        start_wl_o_idx = self.idx_for_wl_out(start_wl)

        ret_array = []

        for idx_i, idx_o in zip(range(start_wl_i_idx, self.wavelength_i_n_samples), 
                                range(start_wl_o_idx, self.wavelength_o_n_samples)):
            ret_array.append(self.data[idx_o, idx_i])

        return ret_array

    def get_excitation_wavelengths(self):
        return np.linspace(self.wavelength_i_start, self.wavelength_i_end, self.wavelength_i_n_samples)

    def get_reflectance_wavelengths(self):
        return np.linspace(self.wavelength_o_start, self.wavelength_o_end, self.wavelength_o_n_samples)

    def idx_for_wl_in(self, wl_in):
        return int(np.floor(wl_in - self.wavelength_i_start) // self.wavelength_i_sampling)

    def idx_for_wl_out(self, wl_out):
        return int(np.floor(wl_out - self.wavelength_o_start) // self.wavelength_o_sampling)

    def wl_for_idx_in(self, wl_idx_in):
        return self.wavelength_i_start + wl_idx_in * self.wavelength_i_sampling

    def wl_for_idx_out(self, wl_idx_out):
        return self.wavelength_o_start + wl_idx_out * self.wavelength_o_sampling

###############################################################################
# Compact Fluorescence - GMM

import struct

from collections import Counter
from scipy.stats import multivariate_normal

class GMM:
    """
    Representation of a Gaussian Mixture Model.

    This class represents a fluorescent spectrum as a Gaussian Mixture Model (GMM). 
    It is responsible for writing files serializing the various parameters of the mixture. 
    It does not perfom the fitting.
    """

    def __init__(self):
        """
        Initialize an empty Gaussian Mixture Model.
        """
        self.means        = []
        self.covs         = []
        self.mixing_coefs = []
        self.bic          = 0.0
        self.scale_attenuation = 1
        self.start_wl, self.step_wl = 0, 0
        self.diagonal     = []

    def save(self, output: pathlib.Path) -> None:
        """
        Saves the parameters of the mixture to a binary file.

        Parameters
        ----------
        output : string
            Path to the file to save.
        """
        n_gaussians = len(self.means)
        size_diagonal = len(self.diagonal)

        # Prepare data for export
        means = []
        covs = []

        for m, c in zip(self.means, self.covs):
            means.append(m[0])
            means.append(m[1])

            covs.append(c[0, 0])
            covs.append(c[0, 1])
            covs.append(c[1, 1])

        with open(output, 'wb') as f:
            b = struct.pack(
                '=I'                        # n_gaussians
                + str(2*n_gaussians) + 'd'  # means
                + str(3*n_gaussians) + 'd'  # covariances
                + str(n_gaussians) + 'd'    # mixing coef
                + 'd'                       # scale_attenuation
                + 'Idd'                     # size_diagonal, start_diagonal, step_diagonal
                + str(size_diagonal) + 'd', # diagonal
                n_gaussians,
                *means, *covs, *self.mixing_coefs,
                self.scale_attenuation,
                size_diagonal, self.start_wl, self.step_wl,
                *self.diagonal
                )

            f.write(b)

    def save_ascii(self, output: pathlib.Path) -> None:
        """ 
        Saves the parameters of the mixture to an ASCII file.

        This method allows to write to a plain text file the parameters of the fit. 
        Its purpose is to allow easy reading of the computed value from a standard text editor.

        Parameters
        ----------
        output : string
            Path to the file to save.
        """
        with open(output, 'w') as f:
            f.write('gmm\n')
            f.write('gaussians:\n')
            f.write('{}\n'.format(len(self.mixing_coefs)))

            f.write('means:\n')
            for mean in self.means:
                f.write('{}, {}\n'.format(mean[0], mean[1]))

            f.write('covs:\n')
            for cov in self.covs:
                f.write('{}, {}, {}, {}\n'.format(cov[0,0], cov[0, 1], cov[1, 0], cov[1, 1]))

            f.write('weights:\n')
            for weigth in self.mixing_coefs:
                f.write('{}\n'.format(weigth))

            f.write('diagonal:\n')
            f.write('{}, {}, {}\n'.format(len(self.diagonal), self.start_wl, self.step_wl))
            for v in self.diagonal:
                f.write('{}\n'.format(v))

            f.write('scaling_factor:\n')
            f.write('{}\n'.format(self.scale_attenuation))           

    def eval_gmm(self, wl_i: float, wl_o: float) -> float:
        """
        Gets the GMM value for specific incident and reemission wavelengths.

        Parameters
        ----------
        wl_i : float
            Incident wavelength in nanometers.
        wl_o : float
            Reemission wavelenght in nanometers.

        Returns
        -------
        Gaussian mixture value for `w_i` to `w_o`.
        """
        res = 0

        for mean, cov, weigth in zip(self.means, self.covs, self.mixing_coefs):
            try:
                res += weigth * multivariate_normal(mean, cov).pdf((wl_i, wl_o))
            except np.linalg.LinAlgError:
                pass

        return res

    def fit_weight(self, fluo_spectrum: FluoSpectrum) -> None:
        """
        Performs fitting of a scaling factor of the current GMM.

        Parameters
        ----------
        fluo_spectrum: FluoSpectrum
            The reference spectrum to use.
        """
        # Filter datasets
        lambda_i = fluo_spectrum.get_excitation_wavelengths()
        lambda_o = fluo_spectrum.get_reflectance_wavelengths()

        original_dataset = np.zeros((len(lambda_o), len(lambda_i)))
        gmm_dataset      = np.zeros((len(lambda_o), len(lambda_i)))
        orig_fluo = fluo_spectrum.get_pure_fluo()

        # Filter datasets
        for i, i_idx in zip(lambda_i, range(len(lambda_i))):
            for o, o_idx in zip(lambda_o, range(len(lambda_o))):
                if o > i:
                    original_dataset[o_idx, i_idx] = max(0, orig_fluo[o_idx, i_idx])
                    gmm_dataset[o_idx, i_idx] = self.eval_gmm(i, o)

        # Scaling factor for the reradiation
        return np.sum(original_dataset) / np.sum(gmm_dataset)

class GMM_Fitting_Base(GMM):
    """
    Class providing methods for initialization of data required by the fitting algorithms.
    """

    def init_fitting_datastructures(self, fluo_spectrum: FluoSpectrum) -> bool:
        """
        Performs initialization of data required by the fitting algorithms.

        Returns
        -------
        fluo_present : boolean signaling whenever fitting should happen (there is a fluorescence reradiation present) or not.
        """
        # Get info on the non fluorescent part
        self.start_wl = max(fluo_spectrum.wavelength_i_start, fluo_spectrum.wavelength_o_start)
        self.step_wl = fluo_spectrum.wavelength_i_sampling
        self.diagonal = fluo_spectrum.get_non_fluo()
        self.means = []
        self.covs = []
        self.mixing_coefs = []
        
        if np.sum(fluo_spectrum.get_pure_fluo_filtered()) < 1e-4:
            # No fluorescence
            self.scale_attenuation = 0
            return False
        else:
            # Get all the data points in reradiation matrix
            x = np.linspace(
                fluo_spectrum.wavelength_i_start, 
                fluo_spectrum.wavelength_i_end, 
                num=fluo_spectrum.wavelength_i_n_samples)

            y = np.linspace(
                fluo_spectrum.wavelength_o_start, 
                fluo_spectrum.wavelength_o_end, 
                num=fluo_spectrum.wavelength_o_n_samples)

            X, Y = np.meshgrid(x, y)

            # Python specific dimension processing
            X = np.ravel(X)
            Y = np.ravel(Y)
            Z = np.ravel(fluo_spectrum.get_pure_fluo())
            training_set = np.vstack([X, Y, Z]).T
            training_set = training_set[training_set[:, 2] > 0.0000]

            # Arrange data for fitting
            self.samples = training_set[:, :-1]
            self.sample_weights = training_set[:, 2]

            return True

class GMM_Weighted_EM(GMM_Fitting_Base):
    def __init__(self, n_gaussians: int, fluo_spectrum: FluoSpectrum):
        if super(GMM_Weighted_EM, self).init_fitting_datastructures(fluo_spectrum):
            # Fit
            model = WeightedGaussianMixture(n_components=n_gaussians, max_iter=1000, random_state=42)
            model.fit(self.samples, sample_weight=self.sample_weights)

            # Save fitting parameters
            self.means = model.means_
            self.covs = model.covariances_
            self.mixing_coefs = model.weights_

            # Find a suitable scaling factor for the reradiation
            self.scale_attenuation = self.fit_weight(fluo_spectrum)

class GMM_Weighted_Bayesian(GMM_Fitting_Base):
    def __init__(self, n_gaussians: int, fluo_spectrum: FluoSpectrum):
        if super(GMM_Weighted_Bayesian, self).init_fitting_datastructures(fluo_spectrum):
            # Fit
            model = WeightedBayesianGaussianMixture(
                weight_concentration_prior_type="dirichlet_process",
                n_components=n_gaussians, 
                max_iter=1000, 
                random_state=42)
            model.fit(self.samples, sample_weight=self.sample_weights)

            # Save fitting parameters

            # Scikit-learn doesn't remove duplicated clusters from Bayesian fitting
            counter = Counter(model.predict(self.samples, sample_weight=self.sample_weights))
            clusters = counter.keys()

            for c in clusters:
                self.means.append(model.means_[c])
                self.covs.append(model.covariances_[c])
                self.mixing_coefs.append(model.weights_[c])

            # Find a suitable scaling factor for the reradiation
            self.scale_attenuation = self.fit_weight(fluo_spectrum)

###############################################################################
# Main

import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser("Compact Fluorescence.", formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        "rfile", 
        type=str, 
        default="", 
        help=(
            "File containing reradiation matrix in native ART or BFC format. "
            "Sampling precision of incident and outgoing radiation must be the same!"
        ),
    )

    parser.add_argument(
        "-m",
        "--m",
        dest="mode",
        type=str,
        choices=["bayes", "em"],
        default="bayes",
        help=(
            "Fitting GMM method to be used. "
            "Weighted Variational Bayesian Inference (\"bayes\") "
            "or Weighted Expectation-Maximization (\"em\"). "
            "Quality of their fits is similar. EM always fits the required amount of gaussians "
            "whether Bayes is allowed to use smaller than the specified number of gaussians."
        ),
    )

    def check_positive(val):
        try:
            ival = int(val)
            if ival <= 0:
                raise
            return ival
        except:
            raise argparse.ArgumentTypeError("\"%s\" is an invalid positive integer value!" % val)

    parser.add_argument(
        "-g",
        "--g",
        dest="gaussians",
        type=check_positive,
        default=8,
        help=(
            "Number of gaussians in the fitted mixture."
        ),
    )

    parser.add_argument(
        "-o",
        "--o",
        dest="outfile",
        type=str,
        default="",
        help=(
            "Output file. If not specified resulting GMM is saved as a \".gmm\" "
            "file under the same directory and filename as the input file."
        ),
    )

    parser.add_argument(
        "-d",
        "--d",
        dest="debug_output",
        action="store_true",
        help=(
            "Save resulting GMM as an ASCII file providing "
            "human-readable output usable for debugging."
        ),
    )

    args = parser.parse_args()

    rfile_path = pathlib.Path(args.rfile).resolve()
    if args.outfile:
        out_path = pathlib.Path(args.outfile).with_suffix(".gmm")
    else:
        out_path = rfile_path.with_suffix(".gmm").resolve()

    fluo_spectrum = FluoSpectrum(rfile_path)

    if args.mode == "bayes":
        fit = GMM_Weighted_Bayesian(args.gaussians, fluo_spectrum)
    else:
        fit = GMM_Weighted_EM(args.gaussians, fluo_spectrum)

    if args.debug_output:
        fit.save_ascii(out_path)
    else:
        fit.save(out_path)
