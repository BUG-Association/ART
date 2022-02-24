/* ===========================================================================

    Copyright (c) The ART Development Team
    --------------------------------------

    For a comprehensive list of the members of the development team, and a
    description of their respective contributions, see the file
    "ART_DeveloperList.txt" that is distributed with the libraries.

    This file is part of the Advanced Rendering Toolkit (ART) libraries.

    ART is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any
    later version.

    ART is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with ART.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================== */


/* ---------------------------------------------------------------------------
    'ARNVAL_CONST_EXPR_GETVALUE_IMPL'
    'ARNVAL_UNARY_EXPR_GETVALUE_IMPL'
    'ARNVAL_BINARY_EXPR_GETVALUE_IMPL'
    'ARNVAL_TERNARY_EXPR_GETVALUE_IMPL'
    'ARNVAL_TERNARY_CONDITIONAL_EXPR_GETVALUE_IMPL'
    'ARNVAL_QUATERNARY_EXPR_GETVALUE_IMPL'
        Standard implementations of get***Value(s) methods for expression nodes
        (constants, operators, functions).
 --------------------------------------------------------------------------- */

#define ARNVAL_CONST_EXPR_GETVALUE_IMPL(_otype, _value) \
    - (unsigned int) get ## _otype ## Values \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (const unsigned int) numberOfValues \
        : (_otype *) outValues \
    { \
        (void) evalEnv; \
        \
        unsigned int actualNumberOfValues = \
            M_MIN(numberOfValues, ARPVALUES_MAX_VALUES); \
        for (unsigned int i = 0; i < actualNumberOfValues; i++) \
            outValues[i] = _value; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) get ## _otype ## Value \
        : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
        : (_otype *) outValue \
    { \
        (void) evalEnv; \
        \
        if (outValue == NULL) \
            return 0; \
        *outValue = _value; \
        return 1; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Values \
        : (const unsigned int) numberOfValues \
        : (_otype *) outValues \
    { \
        unsigned int actualNumberOfValues = \
            M_MIN(numberOfValues, ARPVALUES_MAX_VALUES); \
        for (unsigned int i = 0; i < actualNumberOfValues; i++) \
            outValues[i] = _value; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Value \
        : (_otype *) outValue \
    { \
        if (outValue == NULL) \
            return 0; \
        *outValue = _value; \
        return 1; \
    }

#define ARNVAL_UNARY_EXPR_GETVALUE_IMPL( \
        _itype, \
        _otype, \
        _fmulti,\
        _fsingle \
        ) \
    - (unsigned int) get ## _otype ## Values \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (const unsigned int) numberOfValues \
            : (_otype *) ov \
    { \
        _itype iv[ARPVALUES_MAX_VALUES]; \
        unsigned int actualNumberOfValues = \
            [(ArNode <Arp ## _itype ## Values> *)ARNUNARY_SUBNODE \
                get ## _itype ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv \
                    ]; \
        for (unsigned int i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) get ## _otype ## Value \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (_otype *) ov \
    { \
        _itype iv; \
        unsigned int numberOfValues = \
            [(ArNode <Arp ## _itype ## Values> *)ARNUNARY_SUBNODE  \
                get ## _itype ## Value \
                   :  evalEnv \
                   :  &iv \
                   ]; \
        if (numberOfValues > 0) \
            _fsingle; \
        return numberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Values \
            : (const unsigned int) numberOfValues \
            : (_otype *) ov \
    { \
        _itype iv[ARPVALUES_MAX_VALUES]; \
        unsigned int actualNumberOfValues = \
            [(ArNode <Arp ## _itype ## Values> *)ARNUNARY_SUBNODE \
                getConst ## _itype ## Values \
                    :   numberOfValues \
                    :   iv \
                    ]; \
        for (unsigned int i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Value \
            : (_otype *) ov \
    { \
        _itype iv; \
        unsigned int numberOfValues = \
            [(ArNode <Arp ## _itype ## Values> *)ARNUNARY_SUBNODE  \
                getConst ## _itype ## Value \
                   :  &iv \
                   ]; \
        if (numberOfValues > 0) \
            _fsingle; \
        return numberOfValues; \
    }

#define ARNVAL_BINARY_EXPR_GETVALUE_IMPL( \
        _itype0, \
        _itype1, \
        _otype, \
        _fmulti,\
        _fsingle \
        ) \
    - (unsigned int) get ## _otype ## Values \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (const unsigned int) numberOfValues \
            : (_otype *) ov \
    { \
        _itype0 iv0[ARPVALUES_MAX_VALUES]; \
        _itype1 iv1[ARPVALUES_MAX_VALUES]; \
        unsigned int i; \
        unsigned int actualNumberOfValues = M_MIN( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNBINARY_SUBNODE_0  \
                get ## _itype0 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNBINARY_SUBNODE_1  \
                get ## _itype1 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv1 \
                    ]); \
        for (i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) get ## _otype ## Value \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (_otype *) ov \
    { \
        _itype0 iv0; \
        _itype1 iv1; \
        unsigned int numberOfValues = M_MIN( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNBINARY_SUBNODE_0  \
                get ## _itype0 ## Value \
                    :   evalEnv \
                    :   &iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNBINARY_SUBNODE_1  \
                get ## _itype1 ## Value \
                    :   evalEnv \
                    :   &iv1 \
                    ]); \
        if (numberOfValues > 0) \
            _fsingle; \
        return numberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Values \
            : (const unsigned int) numberOfValues \
            : (_otype *) ov \
    { \
        _itype0 iv0[ARPVALUES_MAX_VALUES]; \
        _itype1 iv1[ARPVALUES_MAX_VALUES]; \
        unsigned int i; \
        unsigned int actualNumberOfValues = M_MIN( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNBINARY_SUBNODE_0  \
                getConst ## _itype0 ## Values \
                    :   numberOfValues \
                    :   iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNBINARY_SUBNODE_1  \
                getConst ## _itype1 ## Values \
                    :   numberOfValues \
                    :   iv1 \
                    ]); \
        for (i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Value \
            : (_otype *) ov \
    { \
        _itype0 iv0; \
        _itype1 iv1; \
        unsigned int numberOfValues = M_MIN( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNBINARY_SUBNODE_0  \
                getConst ## _itype0 ## Value \
                    :   &iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNBINARY_SUBNODE_1  \
                getConst ## _itype1 ## Value \
                    :   &iv1 \
                    ]); \
        if (numberOfValues > 0) \
            _fsingle; \
        return numberOfValues; \
    }

#define ARNVAL_TERNARY_EXPR_GETVALUE_IMPL( \
        _itype0, \
        _itype1, \
        _itype2, \
        _otype, \
        _fmulti,\
        _fsingle \
        ) \
    - (unsigned int) get ## _otype ## Values \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (const unsigned int) numberOfValues \
            : (_otype *) ov \
    { \
        _itype0 iv0[ARPVALUES_MAX_VALUES]; \
        _itype1 iv1[ARPVALUES_MAX_VALUES]; \
        _itype2 iv2[ARPVALUES_MAX_VALUES]; \
        unsigned int i; \
        unsigned int actualNumberOfValues = M_MIN3( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNTERNARY_SUBNODE_0  \
                get ## _itype0 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNTERNARY_SUBNODE_1  \
                get ## _itype1 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNTERNARY_SUBNODE_2  \
                get ## _itype2 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv2 \
                    ]); \
        for (i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) get ## _otype ## Value \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (_otype *) ov \
    { \
        _itype0 iv0; \
        _itype1 iv1; \
        _itype2 iv2; \
        unsigned int actualNumberOfValues = M_MIN3( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNTERNARY_SUBNODE_0  \
                get ## _itype0 ## Value \
                    :   evalEnv \
                    :   &iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNTERNARY_SUBNODE_1  \
                get ## _itype1 ## Value \
                    :   evalEnv \
                    :   &iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNTERNARY_SUBNODE_2  \
                get ## _itype2 ## Value \
                    :   evalEnv \
                    :   &iv2 \
                    ]); \
        if (actualNumberOfValues > 0) \
            _fsingle; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Values \
            : (const unsigned int) numberOfValues \
            : (_otype *) ov \
    { \
        _itype0 iv0[ARPVALUES_MAX_VALUES]; \
        _itype1 iv1[ARPVALUES_MAX_VALUES]; \
        _itype2 iv2[ARPVALUES_MAX_VALUES]; \
        unsigned int i; \
        unsigned int actualNumberOfValues = M_MIN3( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNTERNARY_SUBNODE_0  \
                getConst ## _itype0 ## Values \
                    :   numberOfValues \
                    :   iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNTERNARY_SUBNODE_1  \
                getConst ## _itype1 ## Values \
                    :   numberOfValues \
                    :   iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNTERNARY_SUBNODE_2  \
                getConst ## _itype2 ## Values \
                    :   numberOfValues \
                    :   iv2 \
                    ]); \
        for (i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Value \
            : (_otype *) ov \
    { \
        _itype0 iv0; \
        _itype1 iv1; \
        _itype2 iv2; \
        unsigned int numberOfValues = M_MIN3( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNTERNARY_SUBNODE_0  \
                getConst ## _itype0 ## Value \
                    :   &iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNTERNARY_SUBNODE_1  \
                getConst ## _itype1 ## Value \
                    :   &iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNTERNARY_SUBNODE_2  \
                getConst ## _itype2 ## Value \
                    :   &iv2 \
                    ]); \
        if (numberOfValues > 0) \
            _fsingle; \
        return numberOfValues; \
    }

#define ARNVAL_TERNARY_CONDITIONAL_EXPR_GETVALUE_IMPL( \
        _otype \
        ) \
    - (unsigned int) get ## _otype ## Values \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (const unsigned int) numberOfValues \
            : (      _otype *) ov \
    { \
        int condExpr; \
        [(ArNode <ArpIntValues> *)ARNTERNARY_SUBNODE_0 getIntValues \
                :   evalEnv \
                :   1 \
                : & condExpr \
                ]; \
        if (condExpr) \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_1  \
                    get ## _otype ## Values \
                        :   evalEnv \
                        :   numberOfValues \
                        :   ov \
                        ]; \
        else \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_2  \
                    get ## _otype ## Values \
                        :   evalEnv \
                        :   numberOfValues \
                        :   ov \
                        ]; \
    } \
    \
    - (unsigned int) get ## _otype ## Value \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (      _otype *) ov \
    { \
        int condExpr; \
        [(ArNode <ArpIntValues> *)ARNTERNARY_SUBNODE_0 getIntValue \
                :   evalEnv \
                : & condExpr \
                ]; \
        if (condExpr) \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_1  \
                    get ## _otype ## Value \
                        :   evalEnv \
                        :   ov \
                        ]; \
        else \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_2  \
                    get ## _otype ## Value \
                        :   evalEnv \
                        :   ov \
                        ]; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Values \
            : (const unsigned int) numberOfValues \
            : (      _otype *) ov \
    { \
        int condExpr; \
        unsigned int condRetVal = \
            [(ArNode <ArpIntValues> *)ARNTERNARY_SUBNODE_0 \
                getConstIntValues \
                    :   1 \
                    : & condExpr \
                    ]; \
        if (condRetVal == 0) \
            return 0; \
        \
        if (condExpr) \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_1  \
                    getConst ## _otype ## Values \
                        :   numberOfValues \
                        :   ov \
                        ]; \
        else \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_2  \
                    getConst ## _otype ## Values \
                        :   numberOfValues \
                        :   ov \
                        ]; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Value \
            : (_otype *) ov \
    { \
        int condExpr; \
        unsigned int condRetVal = \
            [(ArNode <ArpIntValues> *)ARNTERNARY_SUBNODE_0 \
                getConstIntValue \
                    : & condExpr \
                    ]; \
        if (condRetVal == 0) \
            return 0; \
        \
        if (condExpr) \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_1  \
                    getConst ## _otype ## Value \
                        :   ov \
                        ]; \
        else \
            return \
                [(ArNode <Arp ## _otype ## Values> *)ARNTERNARY_SUBNODE_2  \
                    getConst ## _otype ## Value \
                        :   ov \
                        ]; \
    }

#define ARNVAL_QUATERNARY_EXPR_GETVALUE_IMPL( \
        _itype0, \
        _itype1, \
        _itype2, \
        _itype3, \
        _otype, \
        _fmulti,\
        _fsingle \
        ) \
    - (unsigned int) get ## _otype ## Values \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (const unsigned int) numberOfValues \
            : (      _otype *) ov \
    { \
        _itype0 iv0[ARPVALUES_MAX_VALUES]; \
        _itype1 iv1[ARPVALUES_MAX_VALUES]; \
        _itype2 iv2[ARPVALUES_MAX_VALUES]; \
        _itype3 iv3[ARPVALUES_MAX_VALUES]; \
        unsigned int i; \
        unsigned int actualNumberOfValues = M_MIN4( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNQUATERNARY_SUBNODE_0  \
                get ## _itype0 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNQUATERNARY_SUBNODE_1  \
                get ## _itype1 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNQUATERNARY_SUBNODE_2  \
                get ## _itype2 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv2 \
                    ], \
            [(ArNode <Arp ## _itype3 ## Values> *)ARNQUATERNARY_SUBNODE_3  \
                get ## _itype3 ## Values \
                    :   evalEnv \
                    :   numberOfValues \
                    :   iv3 \
                    ]); \
        for (i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) get ## _otype ## Value \
            : (const ArcObject <ArpEvaluationEnvironment> *) evalEnv \
            : (_otype *) ov \
    { \
        _itype0 iv0; \
        _itype1 iv1; \
        _itype2 iv2; \
        _itype2 iv3; \
        unsigned int numberOfValues = M_MIN4( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNQUATERNARY_SUBNODE_0  \
                get ## _itype0 ## Value \
                    :   evalEnv \
                    :   &iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNQUATERNARY_SUBNODE_1  \
                get ## _itype1 ## Value \
                    :   evalEnv \
                    :   &iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNQUATERNARY_SUBNODE_2  \
                get ## _itype2 ## Value \
                    :   evalEnv \
                    :   &iv2 \
                    ], \
            [(ArNode <Arp ## _itype3 ## Values> *)ARNQUATERNARY_SUBNODE_3  \
                get ## _itype3 ## Value \
                    :   evalEnv \
                    :   &iv3 \
                    ]); \
        if (numberOfValues > 0) \
            _fsingle; \
        return numberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Values \
            : (const unsigned int) numberOfValues \
            : (_otype *) ov \
    { \
        _itype0 iv0[ARPVALUES_MAX_VALUES]; \
        _itype1 iv1[ARPVALUES_MAX_VALUES]; \
        _itype2 iv2[ARPVALUES_MAX_VALUES]; \
        _itype3 iv3[ARPVALUES_MAX_VALUES]; \
        unsigned int i; \
        unsigned int actualNumberOfValues = M_MIN4( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNQUATERNARY_SUBNODE_0  \
                getConst ## _itype0 ## Values \
                    :   numberOfValues \
                    :   iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNQUATERNARY_SUBNODE_1  \
                getConst ## _itype1 ## Values \
                    :   numberOfValues \
                    :   iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNQUATERNARY_SUBNODE_2  \
                getConst ## _itype2 ## Values \
                    :   numberOfValues \
                    :   iv2 \
                    ], \
            [(ArNode <Arp ## _itype3 ## Values> *)ARNQUATERNARY_SUBNODE_3  \
                getConst ## _itype3 ## Values \
                    :   numberOfValues \
                    :   iv3 \
                    ]); \
        for (i = 0; i < actualNumberOfValues; i++) \
            _fmulti; \
        return actualNumberOfValues; \
    } \
    \
    - (unsigned int) getConst ## _otype ## Value \
            : (_otype *) ov \
    { \
        _itype0 iv0; \
        _itype1 iv1; \
        _itype2 iv2; \
        _itype2 iv3; \
        unsigned int numberOfValues = M_MIN4( \
            [(ArNode <Arp ## _itype0 ## Values> *)ARNQUATERNARY_SUBNODE_0  \
                getConst ## _itype0 ## Value \
                    :   &iv0 \
                    ], \
            [(ArNode <Arp ## _itype1 ## Values> *)ARNQUATERNARY_SUBNODE_1  \
                getConst ## _itype1 ## Value \
                    :   &iv1 \
                    ], \
            [(ArNode <Arp ## _itype2 ## Values> *)ARNQUATERNARY_SUBNODE_2  \
                getConst ## _itype2 ## Value \
                    :   &iv2 \
                    ], \
            [(ArNode <Arp ## _itype3 ## Values> *)ARNQUATERNARY_SUBNODE_3  \
                getConst ## _itype3 ## Value \
                    :   &iv3 \
                    ]); \
        if (numberOfValues > 0) \
            _fsingle; \
        return numberOfValues; \
    }

/* ---------------------------------------------------------------------------
    'ARNVAL_CONST_EXPR_INIT_IMPL'
    'ARNVAL_UNARY_EXPR_INIT_IMPL'
    'ARNVAL_BINARY_EXPR_INIT_IMPL'
    'ARNVAL_TERNARY_EXPR_INIT_IMPL'
    'ARNVAL_QUATERNARY_EXPR_INIT_IMPL'
        Standard implementations of initialization routines for expression
        nodes (constants, operators, functions).
 --------------------------------------------------------------------------- */

#define ARNVAL_CONST_EXPR_INIT_IMPL(_Type, _value_var) \
    - (id) init \
        : (_Type) newValue \
    { \
        self = [ super init ]; \
        \
        if ( self ) \
        { \
            _value_var = newValue; \
        } \
        \
        return self; \
    }

#define ARNVAL_UNARY_EXPR_INIT_IMPL(_itype) \
    - (void) _setupExpr \
    { \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNUNARY_SUBNODE, Arp##_itype##Values); \
    } \
    \
    - (id) init \
            : (ArNode *) newParam \
    { \
        self = \
            [ super init \
                :   HARD_NODE_REFERENCE(newParam) \
                ]; \
        \
        if ( self ) \
        { \
            [ self _setupExpr ]; \
        } \
        \
        return self; \
    }

#define ARNVAL_BINARY_EXPR_INIT_IMPL(_itype0, _itype1) \
    - (void) _setupExpr \
    { \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNBINARY_SUBNODE_0, Arp##_itype0##Values); \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNBINARY_SUBNODE_1, Arp##_itype1##Values); \
    } \
    \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
    { \
        self = \
            [ super init \
                :   HARD_NODE_REFERENCE(newParam0) \
                :   HARD_NODE_REFERENCE(newParam1) \
                ]; \
        \
        if ( self ) \
        { \
            [ self _setupExpr ]; \
        } \
        \
        return self; \
    }

#define ARNVAL_TERNARY_EXPR_INIT_IMPL(_itype0, _itype1, _itype2) \
    - (void) _setupExpr \
    { \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNTERNARY_SUBNODE_0, Arp##_itype0##Values); \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNTERNARY_SUBNODE_1, Arp##_itype1##Values); \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNTERNARY_SUBNODE_2, Arp##_itype2##Values); \
    } \
    \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            : (ArNode *) newParam2 \
    { \
        self = \
            [ super init \
                :   HARD_NODE_REFERENCE(newParam0) \
                :   HARD_NODE_REFERENCE(newParam1) \
                :   HARD_NODE_REFERENCE(newParam2) \
                ]; \
        \
        if ( self ) \
        { \
            [ self _setupExpr ]; \
        } \
        \
        return self; \
    }

#define ARNVAL_QUATERNARY_EXPR_INIT_IMPL(_itype0, _itype1, _itype2, _itype3) \
    - (void) _setupExpr \
    { \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNQUATERNARY_SUBNODE_0, Arp##_itype0##Values); \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNQUATERNARY_SUBNODE_1, Arp##_itype1##Values); \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNQUATERNARY_SUBNODE_2, Arp##_itype2##Values); \
        ART_ERRORHANDLING_MANDATORY_ARPROTOCOL_CHECK( \
                ARNQUATERNARY_SUBNODE_3, Arp##_itype3##Values); \
    } \
    \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            : (ArNode *) newParam2 \
            : (ArNode *) newParam3 \
    { \
        self = \
            [ super init \
                :   HARD_NODE_REFERENCE(newParam0) \
                :   HARD_NODE_REFERENCE(newParam1) \
                :   HARD_NODE_REFERENCE(newParam2) \
                :   HARD_NODE_REFERENCE(newParam3) \
                ]; \
        \
        if ( self ) \
        { \
            [ self _setupExpr ]; \
        } \
        \
        return self; \
    }

/* ---------------------------------------------------------------------------
    'ARNVAL_CONST_INTERFACE'
    'ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE'
    'ARNVAL_UNARY_EXPR_2_TYPES_INTERFACE'
    'ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE'
    'ARNVAL_BINARY_EXPR_2_TYPES_INTERFACE'
    'ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE'
    'ARNVAL_TERNARY_EXPR_2_TYPES_INTERFACE'
    'ARNVAL_QUATERNARY_EXPR_1_TYPE_INTERFACE'
    'ARNVAL_QUATERNARY_EXPR_2_TYPES_INTERFACE'
        Standard interfaces of expression nodes (constants, operators,
        functions, etc).
 --------------------------------------------------------------------------- */

#define ARNVAL_CONST_INTERFACE(_Type, _otype)  \
    @interface ArnVal_const_##_otype \
        : ArNode < ArpConcreteClass, Arp##_Type##Values > \
        {  \
            _Type value;  \
        }  \
        - (id) init  \
            : (_Type) newValue \
        ; \
    @end \

#define ARNVAL_UNARY_EXPR_1_TYPE_INTERFACE(_name, _otype) \
    @interface ArnVal_##_name \
            : ArnUnary < \
                    ArpConcreteClass, \
                    Arp##_otype##Values > \
    - (id) init \
            : (ArNode *) newParam \
            ; \
    @end

#define ARNVAL_UNARY_EXPR_2_TYPES_INTERFACE(_name, _otype0, _otype1) \
    @interface ArnVal_##_name \
            : ArnUnary < \
                    ArpConcreteClass, \
                    Arp##_otype0##Values, Arp##_otype1##Values > \
    - (id) init \
            : (ArNode *) newParam \
            ; \
    @end

#define ARNVAL_BINARY_EXPR_1_TYPE_INTERFACE(_name, _otype) \
    @interface ArnVal_##_name \
            : ArnBinary < \
                    ArpConcreteClass, \
                    Arp##_otype##Values > \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            ; \
    @end

#define ARNVAL_BINARY_EXPR_2_TYPES_INTERFACE(_name, _otype0, _otype1) \
    @interface ArnVal_##_name \
            : ArnBinary < \
                    ArpConcreteClass, \
                    Arp##_otype0##Values, Arp##_otype1##Values > \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            ; \
    @end

#define ARNVAL_TERNARY_EXPR_1_TYPE_INTERFACE(_name, _otype) \
    @interface ArnVal_##_name \
            : ArnTernary < \
                    ArpConcreteClass, \
                    Arp##_otype##Values > \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            : (ArNode *) newParam2 \
            ; \
    @end

#define ARNVAL_TERNARY_EXPR_2_TYPES_INTERFACE(_name, _otype0, _otype1) \
    @interface ArnVal_##_name \
            : ArnTernary < \
                    ArpConcreteClass, \
                    Arp##_otype0##Values, Arp##_otype1##Values > \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            : (ArNode *) newParam2 \
            ; \
    @end

#define ARNVAL_QUATERNARY_EXPR_1_TYPE_INTERFACE(_name, _otype) \
    @interface ArnVal_##_name \
            : ArnQuaternary < \
                    ArpConcreteClass, \
                    Arp##_otype##Values > \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            : (ArNode *) newParam2 \
            : (ArNode *) newParam3 \
            ; \
    @end

#define ARNVAL_QUATERNARY_EXPR_2_TYPES_INTERFACE(_name, _otype0, _otype1) \
    @interface ArnVal_##_name \
            : ArnQuaternary < \
                    ArpConcreteClass, \
                    Arp##_otype0##Values, Arp##_otype1##Values > \
    - (id) init \
            : (ArNode *) newParam0 \
            : (ArNode *) newParam1 \
            : (ArNode *) newParam2 \
            : (ArNode *) newParam3 \
            ; \
    @end

/* ---------------------------------------------------------------------------
    'ARNVAL_CONST_IMPL'
    'ARNVAL_UNARY_EXPR_1_TYPE_IMPL'
    'ARNVAL_UNARY_EXPR_2_TYPES_IMPL'
    'ARNVAL_BINARY_EXPR_1_TYPE_IMPL'
    'ARNVAL_BINARY_EXPR_2_TYPES_IMPL'
    'ARNVAL_TERNARY_EXPR_1_TYPE_IMPL'
    'ARNVAL_TERNARY_EXPR_2_TYPES_IMPL'
        Standard implementations of expression nodes (constants, operators,
        functions).
 --------------------------------------------------------------------------- */

#define ARNVAL_CONST_IMPL( \
        _Type, _otype, _value_type) \
    @implementation ArnVal_const_##_otype \
    ARNVAL_CONST_EXPR_INIT_IMPL(_Type, value) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_const_##_otype) \
    ARPCODING_SIMPLE_1_MEMBER_IMPLEMENTATION(_Type, value) \
    \
    ARPVALUES_NULLARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_##_value_type) \
    \
    ARNVAL_CONST_EXPR_GETVALUE_IMPL(_Type, value) \
    @end

#define ARNVAL_UNARY_EXPR_1_TYPE_IMPL( \
        _itype, _name, \
        _otype, _fmulti, _fsingle) \
    @implementation ArnVal_##_name \
    ARNVAL_UNARY_EXPR_INIT_IMPL(_itype) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_##_otype) \
    ARPVALUES_UNARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_UNARY_EXPR_GETVALUE_IMPL( \
            _itype, \
            _otype, _fmulti, _fsingle) \
    @end

#define ARNVAL_UNARY_EXPR_2_TYPES_IMPL( \
        _itype, _name, \
        _otype0, _fmulti0, _fsingle0, \
        _otype1, _fmulti1, _fsingle1) \
    @implementation ArnVal_##_name \
    ARNVAL_UNARY_EXPR_INIT_IMPL(_itype) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION( \
            arvalue_##_otype0 | arvalue_##_otype1) \
    ARPVALUES_UNARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_UNARY_EXPR_GETVALUE_IMPL( \
            _itype, \
            _otype0, _fmulti0, _fsingle0) \
    ARNVAL_UNARY_EXPR_GETVALUE_IMPL( \
            _itype, \
            _otype1, _fmulti1, _fsingle1) \
    @end

#define ARNVAL_BINARY_EXPR_1_TYPE_IMPL( \
        _itype0, _itype1, _name, \
        _otype, _fmulti, _fsingle) \
    @implementation ArnVal_##_name \
    ARNVAL_BINARY_EXPR_INIT_IMPL(_itype0, _itype1) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_##_otype) \
    ARPVALUES_BINARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_BINARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, \
            _otype, _fmulti, _fsingle) \
@end

#define ARNVAL_BINARY_EXPR_2_TYPES_IMPL( \
        _itype0, _itype1, _name, \
        _otype0, _fmulti0, _fsingle0, \
        _otype1, _fmulti1, _fsingle1) \
    @implementation ArnVal_##_name \
    ARNVAL_BINARY_EXPR_INIT_IMPL(_itype0, _itype1) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION( \
            arvalue_##_otype0 | arvalue_##_otype1) \
    ARPVALUES_BINARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_BINARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, \
            _otype0, _fmulti0, _fsingle0) \
    ARNVAL_BINARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, \
            _otype1, _fmulti1, _fsingle1) \
@end

#define ARNVAL_TERNARY_EXPR_1_TYPE_IMPL( \
        _itype0, _itype1, _itype2, _name, \
        _otype, _fmulti, _fsingle)\
    @implementation ArnVal_##_name \
    ARNVAL_TERNARY_EXPR_INIT_IMPL(_itype0, _itype1, _itype2) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_##_otype) \
    ARPVALUES_TERNARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_TERNARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, _itype2, \
            _otype, _fmulti, _fsingle) \
    @end

#define ARNVAL_TERNARY_EXPR_2_TYPES_IMPL( \
        _itype0, _itype1, _itype2, _name, \
        _otype0, _fmulti0, _fsingle0, \
        _otype1, _fmulti1, _fsingle1) \
    @implementation ArnVal_##_name \
    ARNVAL_TERNARY_EXPR_INIT_IMPL(_itype0, _itype1, _itype2) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION( \
            arvalue_##_otype0 | arvalue_##_otype1) \
    ARPVALUES_TERNARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_TERNARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, _itype2, \
            _otype0, _fmulti0, _fsingle0) \
    ARNVAL_TERNARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, _itype2, \
            _otype1, _fmulti1, _fsingle1) \
    @end

#define ARNVAL_TERNARY_CONDITIONAL_EXPR_IMPL( \
        _oType, _otype \
        ) \
    @implementation ArnVal_i ## _otype ## _otype ## _conditional_ ## _otype \
    ARNVAL_TERNARY_EXPR_INIT_IMPL(Int, _oType, _oType) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION( \
        ArnVal_i ## _otype ## _otype ## _conditional_ ## _otype \
        ) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_ ## _oType) \
    ARPVALUES_TERNARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_TERNARY_CONDITIONAL_EXPR_GETVALUE_IMPL(_oType) \
    @end


#define ARNVAL_QUATERNARY_EXPR_1_TYPE_IMPL( \
        _itype0, _itype1, _itype2, _itype3, _name, \
        _otype, _fmulti, _fsingle)\
    @implementation ArnVal_##_name \
    ARNVAL_QUATERNARY_EXPR_INIT_IMPL(_itype0, _itype1, _itype2, _itype3) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION(arvalue_##_otype) \
    ARPVALUES_QUATERNARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_QUATERNARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, _itype2, _itype3, \
            _otype, _fmulti, _fsingle) \
    @end

#define ARNVAL_QUATERNARY_EXPR_2_TYPES_IMPL( \
        _itype0, _itype1, _itype2, _itype3, _name, \
        _otype0, _fmulti0, _fsingle0, \
        _otype1, _fmulti1, _fsingle1) \
    @implementation ArnVal_##_name \
    ARNVAL_QUATERNARY_EXPR_INIT_IMPL(_itype0, _itype1, _itype2, _itype3) \
    \
    ARPCONCRETECLASS_DEFAULT_IMPLEMENTATION(ArnVal_##_name) \
    ARPCODING_DEFAULT_IMPLEMENTATION \
    \
    ARPVALUES_STANDARD_VALUETYPE_IMPLEMENTATION( \
            arvalue_##_otype0 | arvalue_##_otype1) \
    ARPVALUES_QUATERNARY_EVALENVTYPE_IMPLEMENTATION(arevalenv_none) \
    \
    ARNVAL_QUATERNARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, _itype2, _itype3, \
            _otype0, _fmulti0, _fsingle0) \
    ARNVAL_QUATERNARY_EXPR_GETVALUE_IMPL( \
            _itype0, _itype1, _itype2, _itype3, \
            _otype1, _fmulti1, _fsingle1) \
    @end

/* ---------------------------------------------------------------------------
    The following input & output variables are defined in the context which
    _fmulti or _fsingle macro parameters are expanded (and executed) in.
 --------------------------------------------------------------------------- */

// Multiple values expressions
#define MOV     ov[i]
#define MIV     iv[i]
#define MIV0    iv0[i]
#define MIV1    iv1[i]
#define MIV2    iv2[i]
#define MIV3    iv3[i]

// Single value expressions
#define SOV     (*ov)
#define SIV     iv
#define SIV0    iv0
#define SIV1    iv1
#define SIV2    iv2
#define SIV3    iv3

// ===========================================================================
