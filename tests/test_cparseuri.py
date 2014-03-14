from urllib2 import quote
from urllib2 import unquote

from nose.tools import assert_equal, raises

from cparseuri import decode
from cparseuri import encode

class TestCUriParse(object):

    @classmethod
    def setup_class(cls):
        cls.all_chars = ''.join([chr(i) for i in range(1,256)]) # Skip null byte
        cls.safe_chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_.-/'
        _safe_set = set(cls.safe_chars)
        cls.all_encoded = ''.join([('%%%02x' % i).upper() if chr(i) not in _safe_set else chr(i) for i in range(1,256)])

    @classmethod
    def teardown_class(cls):
        pass

    def setup(self):
        pass

    def teardown(self):
        pass

    def test_encode(self):
        assert_equal(encode(self.all_chars), self.all_encoded)

    def test_encode_urllib2_parity(self):
        assert_equal(encode(self.all_chars), quote(self.all_chars))

    def test_decode(self):
        assert_equal(decode(self.all_encoded), self.all_chars)

    def test_decode_urllib2_parity(self):
        assert_equal(decode(self.all_encoded), unquote(self.all_encoded))

    def test_encode_decode(self):
        assert_equal(decode(encode(self.all_chars)), self.all_chars)

    def test_encode_decode_urllib2_parity(self):
        assert_equal(decode(encode(self.all_chars)), unquote(quote(self.all_chars)))

    def test_safe_encode(self):
        assert_equal(encode(self.safe_chars), self.safe_chars)

    def test_safe_encode_decode(self):
        assert_equal(decode(encode(self.safe_chars)), self.safe_chars)

    def test_safe_subset(self):
        assert_equal(
            sorted([c for c in self.all_chars if encode(c) == c]),
            sorted(list(self.safe_chars))
        )

    def test_decode_backslash_escape(self):
        assert_equal(decode('\x22hello%20world\x22'), '"hello world"')

    def test_decode_quoted_backslash(self):
        assert_equal(decode('%5cx22hello'), '\\x22hello')

    def test_decode_quoted_percent(self):
        assert_equal(decode('%252F'), '%2F')

    def test_encode_empty(self):
        assert_equal(encode(''), '')

    def test_decode_empty(self):
        assert_equal(decode(''), '')

    @raises(TypeError)
    def test_encode_none(self):
        encode(None)

    @raises(TypeError)
    def test_decode_none(self):
        decode(None)

