from enum import Enum, auto


class Op(Enum):

    ###########################################################################
    # Bit-vectors
    ###########################################################################

    # Bit-vector predicates
    BVUGT = auto()
    BVUGE = auto()
    BVSGT = auto()
    BVSGE = auto()
    BVSLT = auto()
    BVSLE = auto()
    BVULT = auto()
    BVULE = auto()
    BVREDAND = auto()
    BVREDOR = auto()

    # Bit-vector arithmetic
    BVNEG = auto()
    BVADD = auto()
    BVSUB = auto()
    BVMUL = auto()
    BVSDIV = auto()
    BVUDIV = auto()
    BVSREM = auto()
    BVUREM = auto()
    BVSMOD = auto()

    # Bit-vector shifts
    BVSHL = auto()
    BVLSHR = auto()
    BVASHR = auto()
    ROTATE_LEFT = auto()
    ROTATE_RIGHT = auto()

    # Bitwise bit-vector operations
    BVNOT = auto()
    BVAND = auto()
    BVOR = auto()
    BVXOR = auto()
    BVNAND = auto()
    BVNOR = auto()
    BVXNOR = auto()

    CONCAT = auto()

    BVITE = auto()
    BVCOMP = auto()

    BVCONST = auto()
    ZERO_EXTEND = auto()
    SIGN_EXTEND = auto()
    EXTRACT = auto()
    REPEAT = auto()

    ###########################################################################
    # Boolean
    ###########################################################################

    NOT = auto()
    AND = auto()
    OR = auto()
    XOR = auto()

    ###########################################################################
    # Arithmetic
    ###########################################################################

    PLUS = auto()
    MINUS = auto()
    MULT = auto()
    LEQ = auto()
    LT = auto()
    GEQ = auto()
    LEFT_SHIFT = auto()
    MOD = auto()

    ###########################################################################
    # Theory-independent
    ###########################################################################

    EQ = auto()
    ITE = auto()

    ###########################################################################
    # Strings
    ###########################################################################

    STRING_CONCAT = auto()
    CONST_STRING = auto()


class BaseSort(Enum):
    Bool = auto()
    BitVec = auto()
    Int = auto()
    String = auto()


class Node:
    def __init__(self, children, sort=None):
        assert all(isinstance(child, Node) for child in children)
        self.children = children
        self.sort = sort
        self.name = None

    def __eq__(self, other):
        if len(self.children) != len(other.children):
            return False

        for c1, c2 in zip(self.children, other.children):
            if c1 != c2:
                return False

        return True


class Sort(Node):
    def __init__(self, base, args):
        super().__init__(args)
        self.base = base

    def __eq__(self, other):
        return self.base == other.base and super().__eq__(other)

    def __hash__(self):
        return hash((self.base, tupe(self.children)))

    def __repr__(self):
        if len(self.children) == 0:
            return '{}'.format(self.base)
        else:
            return '({} {})'.format(
                self.base, ' '.join(str(child) for child in self.children))


class Var(Node):
    def __init__(self, name, sort=None):
        super().__init__([], sort)
        self.name = name

    def __eq__(self, other):
        return self.name == other.name

    def __hash__(self):
        return hash(self.name)

    def __repr__(self):
        return self.name


class CBool(Node):
    def __init__(self, val):
        super().__init__([])
        self.val = val

    def __eq__(self, other):
        assert isinstance(other, Node)
        return isinstance(other, CBool) and self.val == other.val

    def __hash__(self):
        return hash(self.val)

    def __repr__(self):
        return str(self.val)


class IntConst(Node):
    def __init__(self, val):
        super().__init__([])
        self.val = val

    def __eq__(self, other):
        return self.val == other.val

    def __hash__(self):
        return hash(self.val)

    def __repr__(self):
        return str(self.val)


class CString(Node):
    def __init__(self, val):
        super().__init__([])
        self.val = val

    def __eq__(self, other):
        return self.val == other.val

    def __hash__(self):
        return hash(self.val)

    def __repr__(self):
        return f'"{self.val}"'


class App(Node):
    def __init__(self, op, args):
        super().__init__(args)
        self.op = op

    def __eq__(self, other):
        return isinstance(other,
                          Fn) and self.op == other.op and super().__eq__(other)

    def __hash__(self):
        return hash((self.op, tuple(self.children)))

    def __repr__(self):
        return '({} {})'.format(
            self.op, ' '.join(str(child) for child in self.children))
