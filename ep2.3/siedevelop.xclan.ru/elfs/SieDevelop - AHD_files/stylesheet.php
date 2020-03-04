/* Start of CMSMS style sheet 'SieDevelop: Top menu + column' */
/* browsers interpret margin and padding a little differently, we'll remove all default padding and margins and set them later on */
* {
	margin: 0;
	padding: 0;
}
/*Set initial font styles*/
body {
	text-align: left;
	font-family: Verdana, Geneva, Arial, Helvetica, sans-serif;
	font-size: 75.01%;
	line-height: 1em;
}
/*set font size for all divs, this overrides some body rules*/
div {
	font-size: 1em;
}
/*if img is inside "a" it would have borders, we don't want that*/
img {
	border: 0;
}
/*default link styles*/
/* set all links to have underline and bluish color */
a, a:link a:active {
	text-decoration: underline;
/* css validation will give a warning if color is set without background color. this will explicitly tell this element to inherit bg colour from parent element */
	background-color: inherit;
	color: #18507C;
}
a:visited {
	text-decoration: underline;
	background-color: inherit;
	color: #18507C;
/* a different color can be used for visited links */
}
/* remove underline on hover and change color */
a:hover {
	text-decoration: none;
	background-color: inherit;
	color: #385C72;
}
/*****************basic layout *****************/
body {
	margin: 0;
	padding: 0;
/* default text color for entire site*/
	color: #333;
/* you can set your own image and background color here */
	background: #f4f4f4 url(uploads/siedevelop/images/body.png) repeat-x left top;
}
div#pagewrapper {
/* min max width, IE wont understand these, so we will use java script magic in the <head> */
	max-width: 99em;
	min-width: 60em;
/* now that width is set this centers wrapper */
	margin: 0 auto;
	background-color: #fefefe;
	color: black;
}
/* header, we will hide h1 a text and replace it with an image, we assign a height for it so the image wont cut off */
div#header {
/* adjust according your image size */
	height: 100px;
	margin: 0;
	padding: 0;
	/* you can set your own image here, will go behind h1 a image */
	background: #f4f4f4 url(uploads/ngrey/bg_banner.png) repeat-x left top;
/* border just the bottom */
	border-bottom: 1px solid #D9E2E6;
}
div#header h1 a {
/* you can set your own image here */
	background: url(uploads/siedevelop/images/logoSD.png) no-repeat left top;
/* this will make the "a" link a solid shape */
	display: block;
/* adjust according your image size */
	height: 100px;
/* this hides the text */
	text-indent: -999em;
/* old firefox would have shown underline for the link, this explicitly hides it */
	text-decoration: none;
}
div#header h1 {
	margin: 0;
	padding: 0;
/*these keep IE6 from pushing the header to more than the set size*/
	line-height: 0;
	font-size: 0;
/* this will keep IE6 from flickering on hover */
	background: url(uploads/siedevelop/images/logoSD.png) no-repeat left top;
}
div#header h2 {
/* this is where the site name is */
	float: right;
	line-height: 1.2em;
/* this keeps IE6 from not showing the whole text */
	font-size: 1.5em;
/* keeps the size uniform */
	margin: 35px 65px 0px 0px;
/* adjust according your text size */
	color: #f4f4f4;
}
div.crbk {
/* sets all to 0 */
	margin: 0;
	padding: 0;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/mainrtup.gif) no-repeat right bottom;
}
div.breadcrumbs {
/* CSS short hand rule first value is top then right, bottom and left */
	padding: 1em 0em 1em 1em;
/* its good to set font sizes to be relative, this way viewer can change his/her font size */
	font-size: 90%;
/* css shorthand rule will be opened to be "0px 0px 0px 0px" */
	margin: 0px;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/mainleftup.gif) no-repeat left bottom;
}
div.breadcrumbs span.lastitem {
	font-weight: bold;
}
div#search {
/* position for the search box */
	float: right;
/* enough width for the search input box */
	width: 27em;
	text-align: right;
	padding: 0.5em 0 0.2em 0;
	margin: 0 1em;
}
/* a class for Submit button for the search input box */
input.search-button {
	border: none;
	height: 22px;
	width: 53px;
	margin-left: 5px;
	padding: 0px 2px 2px 0px;
/* makes the hover cursor show, you can set your own cursor here */
	cursor: pointer;
/* you can set your own image here */
	background: url(uploads/ngrey/search.gif) no-repeat center center;
}
div#content {
/* some air above and under menu and content */
	margin: 1.5em auto 2em 0;
	padding: 0px;
}
/* this gets all the outside calls that were used on the div#main before  */
div.back1 {
/* this will give room for sidebar to be on the left side, make sure this number is bigger than sidebar width */
	margin-left: 2%; /* 29 */
/* and some air on the right */
	margin-right: 2%;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/mainrt1.gif) no-repeat right top;
}
/* this is an IE6 hack, you may see these through out the CSS */
* html div.back1 {
/* unlike other browser IE6 needs float:right and a width */
	float: right;
	width: 69%;
/* and we take this out or it will stop at the bottom  */
	margin-left: 0%;
/* and some air on the right */
	margin-right: 10px;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/mainrt1.gif) no-repeat right top;
}
div.back2 {
/* you can set your own image here */
	background: url(uploads/siedevelop/images/mainleft1.gif) no-repeat left top;
}
div.back3 {
/* you can set your own image here */
	background: url(uploads/siedevelop/images/wbtmleft.gif) no-repeat left bottom;
}
div#main {
/* this is the last inside div so we set the space inside it to keep all content away from the edges of images/box */
	padding: 10px 15px;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/rtup2.gif) no-repeat right bottom;
}
div#sidebar {
/* set sidebar left. Change to right, float: right; instead, but you will need to change the margins. */
	float: left;
/* sidebar width, if you change this change div.back and/or div.back1 margins */
	width: 26%;
/* FIX IE double margin bug */
	display: inline;
/* the 20px is on the bottom, insures space above footer if longer than content */
	margin: 0px 0px 20px;
	padding: 0px;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/mainrt.gif) no-repeat right top;
}
div#sidebarb {
	padding: 10px 15px 10px 20px;
/* this one is for sidebar with content and no menu */
	background: url(uploads/siedevelop/images/mainrtup.gif) no-repeat right bottom;
}
div#sidebarb div#news {
/* less margin surrounding the news, sidebarb has enough */
	margin: 2em 0 1em 0em;
}
div#sidebara {
	padding: 10px 15px 15px 0px;
/* this one is for sidebar with menu and no content */
	background: url(uploads/siedevelop/images/mainrtup.gif) no-repeat right bottom;
}
div.footback {
/* keep footer below content and menu */
	clear: both;
/* this sets 10px on right to let the right image show, the balance 10px left on next div */
	padding: 0px 10px 0px 0px;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/wfootrt.gif) no-repeat right top;
}
div#footer {
/* this sets 10px on left to balance 10px right on last div */
	padding: 0px 0px 0px 10px;
/* color of text, the link color is set below */
	color: #595959;
/* you can set your own image here */
	background: url(uploads/siedevelop/images/wtopleft.gif) no-repeat left top;
}
div.leftfoot {
	float: left;
	width: 30%;
	margin-left: 20px
}
div#footer p {
/* sets different font size from default */
	font-size: 0.8em;
/* some air for footer */
	padding: 1.5em;
/* centered text */
	text-align: center;
	margin: 0;
}
div#footer p a {
/* footer link would be same color as default we want it same as footer text */
	color: #595959;
}
/* as we hid all hr for accessibility we create new hr with div class="hr" element */
div.hr {
	height: 1px;
	padding: 1em;
	border-bottom: 1px dotted black;
	margin: 1em;
}
/* relational links under content */
div.left49 {
/* combined percentages of left+right equaling 100%  might lead to rounding error on some browser */
	width: 70%;
}
div.right49 {
	float: right;
	width: 29%;
/* set right to keep text on right */
	text-align: right;
}
/********************CONTENT STYLING*********************/
/* HEADINGS */
div#content h1 {
/* font size for h1 */
	font-size: 2em;
	line-height: 1em;
	margin: 0;
}
div#content h2 {
	color: #294B5F;
/* font size for h2 the higher the h number the smaller the font size, most times */
	font-size: 1.5em;
	text-align: left;
/* some air around the text */
	padding-left: 0.5em;
	padding-bottom: 1px;
/* set borders around header */
	border-bottom: 1px solid #899092;
	border-left: 1.1em solid #899092;
/* a larder than h1 line height */
	line-height: 1.5em;
/* and some air under the border */
	margin: 0 0 0.5em 0;
}
div#content h3 {
	color: #294B5F;
	font-size: 1.3em;
	line-height: 1.3em;
	margin: 0 0 0.5em 0;
}
div#content h4 {
	color: #294B5F;
	font-size: 1.2em;
	line-height: 1.3em;
	margin: 0 0 0.25em 0;
}
div#content h5 {
	color: #294B5F;
	font-size: 1.1em;
	line-height: 1.3em;
	margin: 0 0 0.25em 0;
}
h6 {
	color: #294B5F;
	font-size: 1em;
	line-height: 1.3em;
	margin: 0 0 0.25em 0;
}
/* END HEADINGS */
/* TEXT */
p {
/* default p font size, this is set different in some other divs */
	font-size: 1em;
/* some air around p elements */
	margin: 0 0 1.5em 0;
	line-height: 1.4em;
	padding: 0;
}
blockquote {
	border-left: 10px solid #ddd;
	margin-left: 10px;
}
strong, b {
/* explicit setting for these */
	font-weight: bold;
}
em, i {
/* explicit setting for these */
	font-style: italic;
}
/* Wrapping text in <code> tags. Makes CSS not validate */
code, pre {
/* css-3 */
	white-space: pre-wrap;
/* Mozilla, since 1999 */
	white-space: -moz-pre-wrap;
/* Opera 4-6 */
	white-space: -pre-wrap;
/* Opera 7 */
	white-space: -o-pre-wrap;
/* Internet Explorer 5.5+ */
	word-wrap: break-word;
	font-family: "Courier New", Courier, monospace;
	font-size: 1em;
}
pre {
/* black border for pre blocks */
	border: 1px solid #000;
/* set different from surroundings to stand out */
	background-color: #ddd;
	margin: 0 1em 1em 1em;
	padding: 0.5em;
	line-height: 1.5em;
	font-size: 90%;
}
/* Separating the divs on the template explanation page */
div.templatecode {
	margin: 0 0 2.5em;
}
/* END TEXT */
/* LISTS */
/* lists in content need some margins to look nice */
div#main ul,
div#main ol,
div#main dl {
	font-size: 1.0em;
	line-height: 1.4em;
	margin: 0 0 1.5em 0;
}
div#main ul li,
div#main ol li {
	margin: 0 0 0.25em 3em;
}
/* definition lists topics on bold */
div#main dl {
	margin-bottom: 2em;
	padding-bottom: 1em;
	border-bottom: 1px solid #c0c0c0;
}
div#main dl dt {
	font-weight: bold;
	margin: 0 0 0 1em;
}
div#main dl dd {
	margin: 0 0 1em 1em;
}
/* END LISTS */
/* End of 'SieDevelop: Top menu + column' */

