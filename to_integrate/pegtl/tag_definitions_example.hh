// BBCode tags that map to an identical HTML tag.
// "[b]blah[/b]" -> "<b>blah</b>"
SIMPLE_TAG(b)

// BBCode tags that map to a different HTML tag.
// "[quote]blah[/quote]" -> "<blockquote>blah</blockquote>"
SIMPLE_TAG_MAP(quote,"blockquote")

// BBCode tags whose HTML tag should be given some attributes too.
// "[center]blah[/center]" -> "<span style='text-align:center;'>blah</span>"
SIMPLE_TAG_ATTR(center,"span","style='text-align:center;'")

// BBCode tags that have no "inside".
// "[hr]" -> "<hr />"
SELF_CLOSING_TAG(hr,"<hr />")

// Tags that take an argument.
// The &attr; macro will be replaced by that argument.
// The regex is there to limit possible arguments.
// "[style=sparkletext]WOW[/style]" -> "<span class='bbcode_style_sparkletext'>WOW</span>"
ATTR_TAG(style,
	"[A-Za-z0-9_-]*", "Only letters, numbers, underscores and dashes are allowed in style names.",
	"span", "class='bbcode_style_&attr;'")

// Tags that gobble up the text inside, instead of letting it be interpreted as more BBCode.
// The regex is there to limit possible arguments, but in the case of greedy tags, it's optional.
GREEDY_TAG_UNCHECKED(code,"<pre>&body;</pre>")
GREEDY_TAG(
	youtube,
	"[A-Za-z0-9_-]*", "Youtube video ID looks invalid (was expecting something like [youtube]4OjsOxEJRuY[/youtube]).",
	"<iframe width='560' height='315' src='https://www.youtube.com/embed/&body;' frameborder='0' allowfullscreen></iframe>")

/*

// note well: &attr;/&body; will ALWAYS be entity-escaped.
// a &rawattr; could be impl'd but why would you need it???? defeats the purpose
ATTR_TAG(style,"^[A-Za-z0-9_-]*$","span","class='bbcode_style_&attr;'")
GREEDY_TAG(
	youtube,
	"^[A-Za-z0-9_-]*$",
	"<iframe width='560' height='315' src='https://www.youtube.com/embed/&body;' frameborder='0' allowfullscreen></iframe>")
ATTR_TAG(url,".*","a","href='&attr;' ref='nofollow'")
GREEDY_TAG(code,"<pre>&body;</pre>")

*/
