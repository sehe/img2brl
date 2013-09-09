/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  Copyright (C) 2013 Mario Lang <mlang@delysid.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA 
 */

#include <new>
#include <string>
#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/XHTMLDoctype.h>
#include <Magick++/Blob.h>
#include <Magick++/Image.h>

#include <sys/utsname.h>
#include <sys/time.h>

using namespace std;
using namespace cgicc;

static void printForm(Cgicc const &cgi)
{
  static char const *img_file = "img_file";
  cout << form().set("method", "post")
                .set("action", cgi.getEnvironment().getScriptName())
                .set("enctype", "multipart/form-data") << endl
       << cgicc::div()
       << label().set("for", img_file) << "Send an image file: " << label() << endl
       << input().set("id", img_file)
                 .set("type", "file")
                 .set("name", "img")
                 .set("accept", "image/*") << endl
       << cgicc::div()
       << cgicc::div().set("style", "text-align: center") << endl
       << input().set("type", "submit")
                 .set("name", "submit")
                 .set("value", "Translate to Braille") << endl
       << cgicc::div() << endl
       << form() << endl;
}

int main()
{
  try {
    timeval start;
    gettimeofday(&start, NULL);

    Cgicc cgi;
    static char const *text_html_utf8 = "text/html; charset=UTF-8";

    cout << HTTPContentHeader(text_html_utf8)
         << XHTMLDoctype(XHTMLDoctype::eStrict) << endl
         << html().set("xmlns", "http://www.w3.org/1999/xhtml")
                  .set("lang", "en").set("dir", "ltr") << endl
         << head() << endl
         << title() << "Tactile Image Viewer" << title() << endl
         << meta().set("http-equiv", "Content-Type")
                  .set("content", text_html_utf8) << endl
         << link().set("rel", "shortcut icon")
                  .set("href", "favicon.png") << endl
         << head() << endl;
    
    cout << body() << endl;

    const_file_iterator file = cgi.getFile("img");
    if (file != cgi.getFiles().end()) {
      cout << pre() << endl
           << "Data Type: " << file->getDataType() << endl
           << "Filename: " << file->getFilename() << endl;
      Magick::Blob blob(file->getData().data(), file->getData().length());
      Magick::Image image(blob);
      image.write("ubrl:-");
      cout << pre() << endl;
    }

    printForm(cgi);
    cout << hr() << endl;

    cout << cgicc::div().set("style", "text-align: center") << endl
         << a().set("href", "https://github.com/mlang/img2brl")
         << "Source at GitHub"
         << a() << endl
         << cgicc::div() << endl;
    cout << cgicc::div().set("style", "text-align: center") << endl
         << "Configured for " << cgi.getHost();  
    struct utsname info;
    if(uname(&info) != -1) {
      cout << ". Running on " << info.sysname;
      cout << ' ' << info.release << " (";
      cout << info.nodename << ")." << endl;
    }

    // Information on this query
    timeval end;
    gettimeofday(&end, NULL);
    long us = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);

    cout << br() << "Total time for request = " << us << " us";
    cout << " (" << static_cast<double>(us/1000000.0) << " s)";

    // End of document
    cout << cgicc::div() << endl;
    cout << body() << html() << endl;

    // No chance for failure in this example
    return EXIT_SUCCESS;
  }

  // Did any errors occur?
  catch(exception const &e) {
    // Reset all the HTML elements that might have been used to 
    // their initial state so we get valid output
    html::reset(); 	head::reset(); 		body::reset();
    title::reset(); 	h1::reset(); 		h4::reset();
    comment::reset(); 	td::reset(); 		tr::reset(); 
    table::reset();	cgicc::div::reset(); 	p::reset(); 
    a::reset();		h2::reset(); 		colgroup::reset();

    // Output the HTTP headers for an HTML document, and the HTML 4.0 DTD info
    cout << HTTPHTMLHeader() << XHTMLDoctype(XHTMLDoctype::eStrict) << endl;
    cout << html().set("lang","en").set("dir","ltr") << endl;

    // Set up the page's header and title.
    // I will put in lfs to ease reading of the produced HTML. 
    cout << head() << endl;

    // Output the style sheet portion of the header
    cout << style() << comment() << endl;
    cout << "body { color: black; background-color: white; }" << endl;
    cout << "hr.half { width: 60%; align: center; }" << endl;
    cout << "span.red, strong.red { color: red; }" << endl;
    cout << "div.notice { border: solid thin; padding: 1em; margin: 1em 0; "
	 << "background: #ddd; }" << endl;

    cout << comment() << style() << endl;

    cout << title("GNU cgicc exception") << endl;
    cout << head() << endl;
    
    cout << body() << endl;
    
    cout << h1() << "GNU cgi" << span("cc", set("class","red"))
	 << " caught an exception" << h1() << endl; 
  
    cout << cgicc::div().set("align","center").set("class","notice") << endl;

    cout << h2(e.what()) << endl;

    // End of document
    cout << cgicc::div() << endl;
    cout << hr() << endl;
    cout << body() << html() << endl;
    
    return EXIT_SUCCESS;
  }
}
