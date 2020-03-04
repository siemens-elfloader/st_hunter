/* Start of CMSMS style sheet 'SieDevelop: CSSMenu - Horizontal' */
/* by Alexander Endresen and mark and Nuno */
#menu_vert {
/* no margin/padding so it fills the whole div */
	margin: 0;
	padding: 0;
}
.clearb {
/* needed for some browsers */
	clear: both;
}
#menuwrapper {
/* set the background color for the menu here */
	background-color: #243135;
/* IE6 Hack */
	height: 1%;
	width: auto;
/* one border at the top */
	border-top: 1px solid #3F565C;
	margin: 0;
	padding: 0;
}
ul#primary-nav, ul#primary-nav ul {
/* remove any default bullets */
	list-style-type: none;
	margin: 0;
	padding: 0;
}
ul#primary-nav {
/* pushes the menu div up to give room above for background color to show */
	padding-top: 10px;
/* keeps the first menu item off the left side */
	padding-left: 10px;
}
ul#primary-nav ul {
/* make the ul stay in place so when we hover it lets the drops go over the content below else it will push everything below out of the way */
	position: absolute;
/* top being the bottom of the li it comes out of */
	top: auto;
/* keeps it hidden till hover event */
	display: none;
/* same size but different color for each border */
	border-top: 1px solid #C8D3D7;
	border-right: 1px solid #C8D3D7;
	border-bottom: 1px solid #ADC0C7;
	border-left: 1px solid #A5B9C0;
}
ul#primary-nav ul ul {
/* now we move the next level ul down from the top a little for distinction */
	margin-top: 1px;
/* pull it in on the left, helps us not lose the hover effect when going to next level */
	margin-left: -1px;
/* keeps the left side of this ul on the right side of the one it came out of */
	left: 100%;
/* sets the top of it inline with the li it came out of */
	top: 0px;
}
ul#primary-nav li {
/* floating left will set menu items to line up left to right else they will stack top to bottom */
	float: left;
/* no margin/padding keeps them next to each other, the padding will be in the "a" */
	margin: 0px;
	padding: 0px;
}
#primary-nav li li {
/* Set the width of the menu elements at second level. Leaving first level flexible. */
	width: 220px;
/* removes any left margin it may have picked up from the first li */
	margin-left: 0px;
/* keeps them tight to the one above, no missed hovers */
	margin-top: -1px;
/* removes the left float set in first li so these will stack from top down */
	float: none;
/* relative to the ul they are in */
	position: relative;
}
/* set the "a" link look here */
ul#primary-nav li a {
/* specific font size, this could be larger or smaller than default font size */
	font-size: 1em;
/* make sure we keep the font normal */
	font-weight: normal;
/* set default link colors */
	color: #fff;
/* pushes out from the text, sort of like making links a certain size, if you give them a set width and/or height you may limit you ability to have as much text as you need */
	padding: 12px 15px 15px;
	display: block;
/* sets no underline on links */
	text-decoration: none;
}
ul#primary-nav li a:hover {
/* kind of obvious */
	background-color: transparent;
}
ul#primary-nav li li a:hover {
/* this is set to #000, black, below so hover will be white text */
	color: #FFF;
}
ul#primary-nav li a.menuactive {
	color: #000;
/* bold to set it off from non active */
	font-weight: bold;
/* set your image here */
	background:  url(uploads/siedevelop/images/nav.png) repeat-x left 0px;
}
ul#primary-nav li a.menuactive:hover {
	color: #000;
/* keep it the same */
	font-weight: bold;
}
#primary-nav li li a.menuparent span {
/* makes it hold a shape */
	display: block;
/* set your image here, right arrow, 98% over from the left, 100% or 'right' puts it to far */
	background:  url(uploads/siedevelop/images/parent.png) no-repeat 98% center;
}
/* gif for IE6, as it can't handle transparent png */
* html #primary-nav li li a.menuparent span {
/* set your image here, right arrow, 98% over from the left, 100% or 'right' puts it to far */
	background:  url(uploads/siedevelop/images/parent.gif) no-repeat 98% center;
}
ul#primary-nav li ul a {
/* insures alignment */
	text-align: left;
	margin: 0px;
/* keeps it relative to it's container */
	position: relative;
/* less padding than first level no need for large links here */
	padding: 6px 3px 6px 15px;
/* if first level is set to bold this will reset this level */
	font-weight: normal;
/* first level is #FFF/white, we need black to contrast with light background */
	color: #000;
	border-top: 0 none;
	border-right: 0 none;
	border-left: 0 none;
}
ul#primary-nav li ul {
/* very lite grey color, by now you should know what the rest mean */
	background: #F3F5F5;
	margin: 0px;
	padding: 0px;
	position: absolute;
	width: auto;
	height: auto;
	display: none;
	position: absolute;
	z-index: 999;
	border-top: 1px solid #FFFFFF;
	border-bottom: 1px solid #374B51;
/*Info: The opacity property is  CSS3, however, will be valid just in CSS 3.1) http://jigsaw.w3.org/css-validator2) More Options chose CSS3 3) is full validate;)*/
	opacity: 0.95;
/* CSS 3 */
}
ul#primary-nav li ul ul {
/*Info: The opacity property is  CSS3, however, will be valid just in CSS 3.1) http://jigsaw.w3.org/css-validator2) More Options chose CSS3 3) is full validate;)*/
	opacity: 95;
/* CSS 3 */
}
/* Styling the appearance of menu items on hover */
#primary-nav li:hover,
#primary-nav li.menuh,
#primary-nav li.menuparenth,
#primary-nav li.menuactiveh {
/* set your image here, dark grey image */
	background:  url(uploads/siedevelop/images/darknav.png) repeat-x left center;
	color: #000
}
/* The magic - set to work for up to a 3 level menu, but can be increased unlimited, for fourth level add
#primary-nav li:hover ul ul ul,
#primary-nav li.menuparenth ul ul ul,
*/
#primary-nav ul,
#primary-nav li:hover ul,
#primary-nav li:hover ul ul,
#primary-nav li.menuparenth ul,
#primary-nav li.menuparenth ul ul {
	display: none;
}
/* for fourth level add
#primary-nav ul ul ul li:hover ul,
#primary-nav ul ul ul li.menuparenth ul,
*/
#primary-nav li:hover ul,
#primary-nav ul li:hover ul,
#primary-nav ul ul li:hover ul,
#primary-nav li.menuparenth ul,
#primary-nav ul li.menuparenth ul,
#primary-nav ul ul li.menuparenth ul {
	display: block;
}
/* IE6 Hacks */
#primary-nav li li {
	float: left;
	clear: both;
}
#primary-nav li li a {
	height: 1%;
}
/* End of 'SieDevelop: CSSMenu - Horizontal' */

