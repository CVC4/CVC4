#!/usr/bin/env python
#####################
## genkinds.py
## Top contributors (to current version):
##   Makai Mann
## This file is part of the CVC4 project.
## Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
## in the top-level source directory and their institutional affiliations.
## All rights reserved.  See the file COPYING in the top-level source
## directory for licensing information.
##
"""
This script reads CVC4/src/api/cvc4cppkind.h and generates
.pxd and .pxi files which declare all the CVC4 kinds and
implement a Python wrapper for kinds, respectively. The
default names are kinds.pxd / kinds.pxi, but the name is
configurable from the command line with --kinds-file-prefix.

The script is aware of the '#if 0' pattern and will ignore
kinds declared between '#if 0' and '#endif'. It can also
handle nested '#if 0' pairs.
"""

import os
import sys

# the following command in CVC4/build/src/api/python/CMakeFiles/gen-pycvc4-kinds.dir/build.make
# cd CVC4/build/src/api/python && /usr/bin/python3 CVC4/src/api/python/genkinds.py ...
# indicates we are in directory CVC4/build/src/api/python
# so we use ../../../../src/api to access CVC4/src/api/parsekinds.py
sys.path.insert(0, os.path.abspath('../../../../src/api'))

from parsekinds import *
import argparse

#################### Default Filenames ################
DEFAULT_PREFIX = 'kinds'

################ Comments and Macro Tokens ############
PYCOMMENT           = '#'

####################### Code Blocks ###################
CDEF_KIND = "    cdef Kind "

KINDS_PXD_TOP = \
r"""cdef extern from "api/cvc4cppkind.h" namespace "CVC4::api":
    cdef cppclass Kind:
        pass


# Kind declarations: See cvc4cppkind.h for additional information
cdef extern from "api/cvc4cppkind.h" namespace "CVC4::api::Kind":
"""

KINDS_PXI_TOP = \
r"""# distutils: language = c++
# distutils: extra_compile_args = -std=c++11

from cvc4kinds cimport *
import sys
from types import ModuleType

from libcpp.string cimport string
from libcpp.unordered_map cimport unordered_map

# these maps are used for creating a kind
# it is needed for dynamically making a kind
# e.g. for getKind()
cdef unordered_map[int, Kind] int2kind
cdef unordered_map[int, string] int2name

cdef class kind:
    cdef Kind k
    cdef str name
    def __cinit__(self, int kindint):
        self.k = int2kind[kindint]
        self.name = str(int2name[kindint])

    def __eq__(self, kind other):
        return (<int> self.k) == (<int> other.k)

    def __ne__(self, kind other):
        return (<int> self.k) != (<int> other.k)

    def __hash__(self):
        return hash((<int> self.k, self.name))

    def __str__(self):
        return self.name

    def __repr__(self):
        return self.name

    def as_int(self):
        return <int> self.k

# create a kinds submodule
kinds = ModuleType('kinds')
kinds.__file__ = kinds.__name__ + ".py"
"""

KINDS_ATTR_TEMPLATE = \
r"""
int2kind[<int> {kind}] = {kind}
int2name[<int> {kind}] = b"{name}"
cdef kind {name} = kind(<int> {kind})
setattr(kinds, "{name}", {name})
"""


def gen_pxd(parser: KindsParser, filename):
    f = open(filename, "w")
    f.write(KINDS_PXD_TOP)
    # include the format_name docstring in the generated file
    # could be helpful for users to see the formatting rules
    for line in parser.format_name.__doc__.split(NL):
        f.write(PYCOMMENT)
        if not line.isspace():
            f.write(line)
        f.write(NL)
    for kind in parser.kinds:
        f.write(CDEF_KIND + kind + NL)
    f.close()

def gen_pxi(parser : KindsParser, filename):
    f = open(filename, "w")
    pxi = KINDS_PXI_TOP
    for kind, name in parser.kinds.items():
        pxi += KINDS_ATTR_TEMPLATE.format(name=name, kind=kind)
    f.write(pxi)
    f.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser('Read a kinds header file and generate a '
                         'corresponding pxd file, with simplified kind names.')
    parser.add_argument('--kinds-header', metavar='<KINDS_HEADER>',
                        help='The header file to read kinds from',
                        default=DEFAULT_HEADER)
    parser.add_argument('--kinds-file-prefix', metavar='<KIND_FILE_PREFIX>',
                        help='The prefix for the .pxd and .pxi files to write '
                        'the Cython declarations to.',
                        default=DEFAULT_PREFIX)

    args               = parser.parse_args()
    kinds_header       = args.kinds_header
    kinds_file_prefix  = args.kinds_file_prefix

    kp = KindsParser()
    kp.parse(kinds_header)

    gen_pxd(kp, kinds_file_prefix + ".pxd")
    gen_pxi(kp, kinds_file_prefix + ".pxi")
