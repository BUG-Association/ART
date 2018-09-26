%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%
%%  art.cls --- Class File
%%  derived from algodat.cls, but based on report.cls
%%  Zur Verwendung am Institut fuer Computergraphik der TU Wien
%%  fuer die Skripten zu den VO Algorithmen und Datenstrukturen 1/2
%%
%%  LaTeX2e-Version von Georg ZOTTI, MNr. 9126124/881
%%             
%%
%%  Status: 30.8.1998, 1900MESZ --- gzotti findet die Form perfekt!
%%
%% 2005-jan-24: gz made art.cls as derived class file. Many omissions/simplifications.

%% ALL options passed to book.cls, which is called with [onecolumn,10pt,twoside,openright,a4paper]


%% OPTIONEN: entwurf -> diagonal ueber alle Seiten gedrucktes "ENTWURF",
%%                      Makro \draftcopy{#1} setzt Entwurfs-Anmerkungen an den
%%                      Rand. Andernfalls werden diese Anmerkungen ignoriert.
%%           layout  -> Druckt eine Test-Doppelseite zur Kontrolle der
%%                      Seitenausrichtung
%%           draft   -> Wie draft bei book-Style: eps-Graphik nur als
%%                      Kaestchen, overfull boxes mit schw. Block gekennzeichnet.


%%%%%% Identification Part
\NeedsTeXFormat{LaTeX2e}[2000/06/01] % probably also earlier versions?
\ProvidesClass{art}[2005/01/24 Document class for ART documentation, TU Wien]
\message{2005/01/24 art.cls, G. Zotti, Institute of Computer Graphics, TU Wien}

%%%%%% Initial Code Part

%%%%%% Declaration of Options

% forward all options to book
%\DeclareOption*{\message{algodat.cls: Option "\CurrentOption" nicht verfuegbar! Gueltig: "draft", "entwurf", "layout".}}
\DeclareOption*{\PassOptionsToClass{*}{report}}%article}}
%\DeclareOption{draft}{\PassOptionsToClass{draft}{book}}

%%%%%% Execution Of Options

%\ExecuteOptions{entwurf,draft,layout}
\ProcessOptions

%%%%%% Package Loading
\LoadClass[onecolumn,10pt,twoside,a4paper]{report}%article}
\RequirePackage[austrian,english]{babel}
\RequirePackage[latin1]{inputenc}
\RequirePackage[T1]{fontenc}
% SELECT ONE OF THESE:
%\RequirePackage{mathptmx}\RequirePackage[scaled=.90]{helvet}\RequirePackage{courier} % PostScript-Fonts Times/Helvetica/Courier
\RequirePackage{mathpazo}\RequirePackage[scaled=.95]{helvet}\RequirePackage{courier} % PS-Fonts Palatino/Helvetica/Courier
%\RequirePackage{newcent}\RequirePackage[scaled=.95]{helvet}\RequirePackage{courier}  % PS-Fonts NewCenturySchoolbook/AvantGarde/Courier
%\RequirePackage{bookman}\RequirePackage[scaled=.95]{helvet}\RequirePackage{courier}  % PS-Fonts Bookman/AvantGarde/Courier
\RequirePackage{wasysym}            % some nice symbols. See wasysym.dvi
\RequirePackage{calc}               % fuer einfachere Berechnungen der Dimensionen
\RequirePackage{ifthen}             % fuer \Siehe, \Ord, etc.
\RequirePackage{graphicx}           % Einbinden von Graphiken
\RequirePackage{fancyhdr}           % schoene Kopfzeilen
\RequirePackage{array}              % Formeln schoen arrangiert
\RequirePackage{tabularx}           % verbessertes tabular-Env.
\RequirePackage{multirow}           % mehrzeilige Eintraege in Tabellen
\RequirePackage{hhline}             % bessere Tabellen-Linien
\RequirePackage{dcolumn}            % bessere Tabellen-Spalten
%\RequirePackage[intlimits]{amsmath}% \ld u.v.a.m.; intlimits fuer Integral
%\RequirePackage{float}             % fuer listing-floats
%\RequirePackage{boxedminipage}     % minipage mit Rahmen!
\RequirePackage{alltt}              % fuer Listings
\RequirePackage[obeyspaces]{url}   % fuer URLs mit gutem Umbruch (und Leerzeichen)!
%\RequirePackage{url}                % well-formatted URLs (with line wrap)
\RequirePackage[breaklinks]{hyperref}           % stuff for the hypperef. SHOULD BE THE LAST LOADED PACKAGE!
%%
%[GZ]%\RequirePackage{program}      % Zum Setzen der Programmbeispiele. 
%[GZ]%\NumberPrograms{true|false} % Zeilennummern (nur 1 Wort ohne Klammern!!)
%%% Das Package program ist leider doch nicht so gut, es sollte modifiziert
%%% werden. Dzt. wird einfach eine Variante mit {alltt} verwendet.
%%
%%

%%%%%% Main Code


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% SEITENLAYOUT-PARAMETER %%
%%
\setcounter{secnumdepth}{3}     % keine \subsection-Nummern mehr!
\setcounter{tocdepth}{3}        % Inhaltsverzeichnis bis \sections
\normalsize
\setlength{\evensidemargin}{0mm}        % lSeite, lRand = 1 inch+0mm
\setlength{\oddsidemargin}{0mm}         % rSeite, lRand = 1 inch+0mm
\setlength{\topmargin}{0mm}             % ob. Rand zur Kopfzeile = 1 inch+0mm
\setlength{\headheight}{2\baselineskip} % Kopfhoehe
\setlength{\headsep}{1.5\baselineskip}  % Zwischen Kopf und Text
\setlength{\textheight}{52\baselineskip+\topskip}       % ca 21.17cm
\setlength{\textwidth}{160mm}           % Textbreite
\setlength{\marginparsep}{4mm}          % Zwischen Randnote und Text (SELTEN!)
\setlength{\marginparwidth}{15mm}       % Platz fuer Randnoten (SELTEN..NIE)
\setlength{\footskip}{10mm}             % vert. Abstd. Fussnote vom Textboden
\setlength{\parskip}{0.2\baselineskip %
                     plus 0.5\baselineskip 
                     minus 0.2\baselineskip} % Abstand zw. Absaetzen
%[GZ]%\setlength{\parskip}{1ex plus 0.5ex minus 0.2ex}
\setlength{\parindent}{0pt}
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% LAYOUT fuer Headers und Footers %%
%%
\pagestyle{fancy}
%%[GZ]% folg. 2 Zeilen -> Header ueberrage Randnoten:
%%[GZ]%\addtolength{\headwidth}{\marginparsep}   
%%[GZ]%\addtolength{\headwidth}{\marginparwidth} 
%%%
%\setlength{\headrulewidth}{0.4pt}    % default: 0.4pt
%\setlength{\footrulewidth}{0pt}      % default: 0pt
%\setlength{\plainheadrulewidth}{0pt} % default: 0pt
%\setlength{\plainfootrulewidth}{0pt} % default: 0pt

\renewcommand{\@pnumwidth}{2.4em}    % TableOfContents etc.: Breite der Nummernbox
%% Originaldefinition fuer \footnote-text, nur \quad eingefuegt!
\renewcommand{\@makefntext}[1]{\parindent 1em\noindent%
                            \hb@xt@ 1.8em{\hss \@makefnmark }\quad #1%
                           }
%% Use symbols (star, dagger, ...), not letters in minipages
\renewcommand{\thempfootnote}{\fnsymbol{mpfootnote}} 

%\renewcommand{\sectionmark}[1]{\markright{\thesection\ \ \ #1}}
\lhead[\fancyplain{}{\bfseries\thepage}]{\fancyplain{}{\bfseries\rightmark}}
\rhead[\fancyplain{}{\bfseries\leftmark}]{\fancyplain{}{\bfseries\thepage}}
\cfoot{}

% \leftmark should print Part name.

%[GZ]% \Preservebackslash vor \ragged(left|right),\centering rettet \\-Kommando
%\newcommand{\PreserveBackslash}[1]{\let\temp=\\#1\let\\=\temp}%LaTeX-Comp.,p.108

%% avoid singular lines:
\widowpenalty=10000
\displaywidowpenalty=10000
\clubpenalty=10000

%% LaTeX-Hacks!
%
%[GZ]% \clearemptydoublepage (LaTeX Comp.p93)  
%[GZ]%  sorgt fuer leere linke Seiten bei Kapitelwechsel
\newcommand{\clearemptydoublepage}{\newpage{\pagestyle{empty}\cleardoublepage}}
\AtEndDocument{\clearemptydoublepage}

%% Und jetzt: Einige abgeaenderte LaTeX-Makros! Stand: teTeX 12/96

%%% \chapter jetzt mit inkludiertem \clearemptydoublepage
%\renewcommand{\chapter}{\if@openright \clearemptydoublepage \else \clearpage \fi %
% \thispagestyle {plain}\global \@topnum \z@ \@afterindentfalse \secdef \@chapter \@schapter}

%\renewcommand{\frontmatter}%
% {\if@openright \clearemptydoublepage \else \clearpage \fi%
%  \@mainmatterfalse \pagenumbering{roman}}

%\renewcommand{\mainmatter}%
% {\if@openright \clearemptydoublepage \else \clearpage \fi%
%  \@mainmattertrue \pagenumbering{arabic}}

%\renewcommand{\backmatter}% \clearemptydoublepage, Roman numbers und \chapter 
% {\if@openright \clearemptydoublepage \else \clearpage \fi 
%  \@mainmatterfalse % 
%  \pagenumbering{Roman}%setcounter{page}{1}\renewcommand{\thepage}{\Roman{page}}%
%  \renewcommand{\chapter}{\if@openright \clearemptydoublepage \else \clearpage \fi %
%  \thispagestyle {plain}\global \@topnum \z@ \@afterindentfalse \secdef \@chapter \@schapter}
% }

%% \appendix so, daß Chapters im Appendix jeweils mit Seite 1 beginnen.
%% Seitenbeschriftung (\thepage) mit A-1, A-2, ..., B-1 etc. 

%\renewcommand{\appendix}{\par \setcounter {chapter}{0}%
%   \setcounter {section}{0} \renewcommand \@chapapp {\appendixname }%
%   \renewcommand \thechapter {\@Alph \c@chapter }%
%% Seitenbezeichnung aendern:
%  \renewcommand{\thepage}{\Alph{chapter}--\arabic{page}}%
%% Jetzt \chapter nochmals umdefinieren!
%  \renewcommand{\chapter}{\if@openright \clearemptydoublepage \else \clearpage \fi %
%  \setcounter{page}{1}%
% \thispagestyle {plain}\global \@topnum \z@ \@afterindentfalse \secdef \@chapter \@schapter}
%}


%%% NEW COLUMN TYPES
%\newcolumntype{d}{D{.}{.}{2}} % Decimal alignment, 2 decimals after point
%\newcolumntype{C}{>{$}c<{$}}  % centered-math
%\newcolumntype{L}{>{$}l<{$}}  % left-math
%\newcolumntype{R}{>{$}r<{$}}  % right-math
%\newcolumntype{t}{>{\tt }c}   % centered tt


%% A few Font-Markups

\newcommand{\name}[1]{\textsc{#1}}  % SmallCaps
\newcommand{\filename}[1]{\texttt{#1}}
\newcommand{\command}[1]{\texttt{#1}}  %for commands like gcc, ams ...
\newcommand{\source}[1]{\texttt{#1}}  %for commands like gcc, ams ...
\newcommand{\option}[1]{\texttt{#1}}   %for command options. Usually formatted equally, we just separate semantically. 
\newcommand{\variable}[1]{\texttt{#1}} %for environment variables -- OLD, will be removed!
\newcommand{\envvar}[1]{\texttt{\$#1}} %for environment variables
\newcommand{\class}[1]{\textsf{#1}}  % class names
\newcommand{\type}[1]{\texttt{#1}} %for data types
\newcommand{\protocol}[1]{\textsf{#1}}  % protocol names
\newcommand{\function}[1]{\texttt{#1}}  % function names
\newcommand{\var}[1]{\texttt{#1}}      %for program variables
\newcommand{\important}[1]{\textit{#1}} %for important lines like 'you should not ... '
\newcommand{\hhref}[1]{\href{#1}{\url{#1}}}   % hyperref without duplication, plus well-formatted URL

\newcommand{\mailto}[1]{\texttt{<\url{mailto:#1}>}}
\newcommand{\Note}[1]{\textbf{Note:} #1}

%\newcommand{\Book}[1]{\textsf{#1}}
%\newcommand{\etc}{\textit{etc}}         % in der Literatur empfohlen!
%\newcommand{\qed}{\hfill{\rmfamily\bfseries\itshape q.e.d.}}

%%
%% 2/3-part references
%% \See{chaprefname}{chaptername}{sectionname}
%% If sectionname (really empty) (={}), no -- is shown
%%
%\newcommand{\See}[3]{\ifthenelse{\equal{#3}{}}        
%                                {\ifthenelse{\equal{#2}{}}
%                                        {\textbf{\ref{#1}}}
%                                        {\textbf{\ref{#1}~{#2}}}}
%                                {\ifthenelse{\equal{#2}{}}
%                                        {\textbf{\ref{#1}~-- {#3}}}
%                                        {\textbf{\ref{#1}~{#2}~-- {#3}}}}}
%
%
%
%% Sprueche am Kapitel-/Abschnittsanfang...
%%
%\newcommand{\Motto}[2]% Am Kapitelanfang rechtsbuendige Sprueche mit Autoren
%  {{\hfill\begin{minipage}{.5\linewidth}%
%    \begin{flushleft}{\small{#1}}\end{flushleft}%
%    \vspace{-0.5\baselineskip}%
%    \textit{\small{#2}}\end{minipage}}\medskip\par}
%
%
%% \WerWannWas{#1=Name}{#2=von[-bis]}{#3=Leistungen}
%%  Nett fuer Fussnoten, in denen Persoenlichkeiten vorgestellt werden.
%%  Jahreszahlen mit \J{} anschreiben fuer oldstyle-Zahlen!; #3 kann leer bleiben! 
%%
%\newcommand{\WerWannWas}[3]{%
%        \newcommand{\J}[1]{$\oldstylenums{##1}$}%
%        \name{#1}, #2\ifthenelse{\equal{#3}{}}{}{, #3}}
%
%
%
%%
%% \Tstrut macht Tabellen lesbarer, indem die Zeilen tiefer werden:
%% Jede Tabellenzeile mit eingebautem Tstrut hat mindestens diese Hoehe.
%%
%\newcommand{\Tstrut}{\rule[-2.5mm]{0pt}{6.5mm}}
%
%
%%
%\newenvironment{program}        % Programme
%        {\begin{alltt}}
%        {\end{alltt}}
%%%
%%% programbox macht eine eingerueckte Box -> Kein Seitenumbruch!
%\newenvironment{programbox}
%        {\vspace{2\parskip plus \baselineskip minus \parskip}\par\noindent%
%        \hfill\begin{minipage}{0.96\linewidth}\begin{alltt}}
%        {\end{alltt}\end{minipage}%
%        \vspace{2\parskip plus \baselineskip minus \parskip}\par\noindent}
%%
%%% Falls Programm zu \Beispiel{} gehoert, kann man es leider nicht direkt
%%% innerhalb von \Beispiel{} in {programbox} setzen, da sonst Zeilen- 
%%% umbrueche verloren gehen. (Kollision von alltt, minipage und 
%%% description-\item ?)
%%% Also: Nach \Beispiel-Ende; Fast wie programbox, nur weiter eingerueckt:
%\newenvironment{Beispielprogramm}
%        {\vspace{2\parskip plus \baselineskip minus \parskip}\par\noindent%
%        \hfill\begin{minipage}{0.93\linewidth}\begin{alltt}}    
%        {\end{alltt}\end{minipage}%
%         \vspace{2\parskip plus \baselineskip minus \parskip}\par\noindent}
%%% \Ptr fuer Pointer-Zeichen (^); 
%%% Verwende immer \Ptr{} oder {\Ptr} zur Vermeidung von whitespace dahinter!
%\newcommand{\Ptr}{\mbox{\texttt{\^{}}}} 
%%% Innerhalb Programmen/Listings:
%\newcommand{\comment}[1]  {$\{$\textsl{#1}$\}$}% Kommentar
%\newcommand{\rcomment}[1]{\hfill$\{$\textsl{#1}$\}$}% rechtsbuendiger Kommentar
%\newcommand{\llcomment}[1]{$\{$\textsl{#1\hfill}$\}$} % m. schl. Klammer rechts
%\newcommand{\rlcomment}[1]{$\{$\textsl{\hfill#1}$\}$} % m. oeff. Klammer links
%\newcommand{\Pseudo}[1]{%  Pseudocode: roman-it mit tt-it \Pasop-s
%   \renewcommand{\Pasop}[1]{\texttt{\textit{##1}}}%
%   \textrm{\textit{#1}}} 
%%%


%%
%%%%%%%%%%%%%%%%%%%%%%%%%% Do not use classic floats directly! %%%%%%%%%%
%%%
%\floatstyle{plain}      % boxed/plain/ruled
%\floatplacement{figure}{htbp}\restylefloat{figure}
%\floatplacement{table}{htbp}\restylefloat{table}
%\newfloat{ADListing}{htbp}{lol}[chapter]\floatname{ADListing}{Listing}
%\renewcommand{\textfraction}{.1}        % Fuellgrade fuer float-pages
%\renewcommand{\topfraction}{.85}\setcounter{topnumber}{3}
%\renewcommand{\bottomfraction}{.85}
%\renewcommand{\floatpagefraction}{.85}  % s. LaTeX Comp. p.142
%%%
%%%%%% FUER FLOATS IMMER VERWENDEN: Listing, Tabelle, Abbildung: 
%%% Setzen einheitliche Abstaende sowie \label{#2} und \caption[#1]{#3}  
%%% Achtung: Die deutschen "active characters": "s, "a, "o, "u muessen 
%%% in den captions hier wieder im Standard-LaTeX-Stil \ss{}, \"a, \"o, \"u 
%%% gesetzt werden. Dies duerfte ein Fehler im "float"-package sein.
%%% Warnung: "\ss " am Wortende als "\ss{} " oder "\ss\ " schreiben!
%%% Verwendung: \begin{_envname_}[ShortCaption]{label}{caption}
%%%
%\newenvironment{Listing}[3][]%
%        {\begin{ADListing}%
%         \ifthenelse{\equal{#1}{}}%     % 1. Arg: Kurztitel f. Index
%           {\protect\caption{#3}}%              % normal
%           {\protect\caption[#1]{#3}}%          % mit Kurztitel
%         \label{#2}%
%         \vspace*{1mm}\begin{programbox}}%
%        {\end{programbox}\end{ADListing}}%
%\newenvironment{Tabelle}[3][]%
%        {\begin{table}%\vspace*{1mm}%
%         \ifthenelse{\equal{#1}{}}%     % 1. Arg: Kurztitel f. Index
%           {\protect\caption{#3}}%              % normal
%           {\protect\caption[#1]{#3}}%          % mit Kurztitel
%         \label{#2}\begin{center}}%
%        {\end{center}\end{table}}%
%\newenvironment{Abbildung}[3][]%
%        {\begin{figure}%\vspace*{1mm}%
%         \ifthenelse{\equal{#1}{}}%     % 1. Arg: Kurztitel f. Index
%           {\protect\caption{#3}}%              % normal
%           {\protect\caption[#1]{#3}}%          % mit Kurztitel
%         \label{#2}\begin{boxedminipage}{\linewidth}\begin{center}}%
%        {\end{center}\end{boxedminipage}\end{figure}}%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%

%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%
%% orig from report.cls
\renewcommand\chapter{%\if@openright\cleardoublepage\else\clearpage\fi
                    %\thispagestyle{plain}%
                    \global\@topnum\z@
                    \@afterindentfalse
                    \secdef\@chapter\@schapter}
\renewcommand{\@makechapterhead}[1]{%
  {\parindent \z@ \raggedright \normalfont
    \vspace{15\p@}%
    \interlinepenalty\@M
    \ifnum \c@secnumdepth >\m@ne
        \LARGE \bfseries \thechapter\quad #1\par\nobreak
     \else
        \LARGE \bfseries #1\par\nobreak
    \fi
    \vskip 12\p@
  }}


%%% Local Variables: 
%%% mode: latex
%%% TeX-master: t
%%% End: 






