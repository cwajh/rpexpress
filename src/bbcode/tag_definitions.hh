// For examples of how this header file is supposed to work, see tag_definitions_example.hh

SIMPLE_TAG(b)
SIMPLE_TAG(i)
SIMPLE_TAG(u)
SIMPLE_TAG(s)
SIMPLE_TAG(sup)
SIMPLE_TAG(sub)
SIMPLE_TAG(ul)
SIMPLE_TAG(ol)
SIMPLE_TAG(li)
// Supporting [*] would be nice, but the method of doing it (self-closing semi-greedy tag) seems... hazardous.

SIMPLE_TAG_MAP(quote,"blockquote")

SIMPLE_TAG_ATTR(left,"span","style='text-align:left;'")
SIMPLE_TAG_ATTR(center,"span","style='text-align:center;'")
SIMPLE_TAG_ATTR(right,"span","style='text-align:right;'")

SELF_CLOSING_TAG(hr,"<hr />")

ATTR_TAG(size,
	"[1-7]", "Only font sizes from 1 to 7 are allowed.",
	"span", "class='bbcode_size_&attr;'"
)
ATTR_TAG(color,
	"(#[0-9a-fA-F]*)|([a-zA-Z0-9_-]*)", "Colors must either be a name (like \"turquoise\") or an RGB value (like \"#40e0d0\").",
	"span", "style='color: &attr;;'"
)
ATTR_TAG(font,
	"[A-Za-z0-9 _-]*", "This font name has unexpected symbols in it. If you're sure it's actually the name of the font, contact technical support.",
	"span", "style='font-family: \"&attr;\"'"
)
ATTR_TAG(style,
	"[A-Za-z0-9_-]*", "Only letters, numbers, underscores and dashes are allowed in style names.",
	"span", "class='bbcode_style_&attr;'"
)
ATTR_TAG(url,
	".*", "You shouldn't be seeing this \"URL BBCODE Tag Attribute Validation\" error. Contact tech support.",
	"a", "rel='nofollow' href='&attr;'"
)

GREEDY_TAG_UNCHECKED(code,"<pre class='bbcode_codeblock'>&body;</pre>")
GREEDY_TAG_UNCHECKED(img,"<img class='bbcode_image' src='&body;' />")
GREEDY_TAG(
	youtube,
	"[A-Za-z0-9_-]*", "Youtube video ID looks invalid (was expecting something like [youtube]4OjsOxEJRuY[/youtube]).",
	"<iframe width='560' height='315' src='https://www.youtube.com/embed/&body;' frameborder='0' allowfullscreen></iframe>"
)
// TODO: muse/mun need to hit the database somehow.


