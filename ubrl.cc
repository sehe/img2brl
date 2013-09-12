#include "ubrl.h"
#include <Magick++/Blob.h>
#define BOOST_SPIRIT_NO_PREDEFINED_TERMINALS
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_eol.hpp>
#include <boost/spirit/include/qi_eoi.hpp>
#include <boost/spirit/include/qi_expect.hpp>
#include <boost/spirit/include/qi_kleene.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_parse_attr.hpp>
#include <boost/spirit/include/qi_uint.hpp>

ubrl::ubrl(Magick::Image &image)
{
  Magick::Blob blob;
  image.write(&blob, "ubrl");

  using namespace boost::spirit::qi;
  char const *begin = static_cast<char const *>(blob.data());
  char const *const end = begin + blob.length();
  char_type char_;
  eoi_type eoi;
  eol_type eol;
  lit_type lit;
  uint_type uint_;

  parse( begin, end
       , lit("Width: ") > uint_ > eol
           > "Height: " > uint_ > eol
           > eol
           > *char_ > eoi
       , w, h, data);
}
