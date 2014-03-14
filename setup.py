from distutils.core import setup, Extension

setup(
    name='CParseURI',
    version = '0.0.0',
    description = 'C URI Parser',
    ext_modules = [Extension('cparseuri', sources=['src/cparseuri.c'])]
)
