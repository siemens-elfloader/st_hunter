/* Start of CMSMS style sheet 'Siedevelop: Print' */
/*
Sections that are hidden when printing the page. We only want the content printed.
*/


body {
color: #000 !important; /* we want everything in black */
background-color:#fff !important; /* on white background */
font-family:arial; /* arial is nice to read ;) */
border:0 !important; /* no borders thanks */
}

/* This affects every tag */
* {
border:0 !important; /* again no borders on printouts */
}

/* 
no need for accessibility on printout. 
Mark all your elements in content you 
dont want to get printed with class="noprint"
*/
.accessibility,
.noprint
 {
display:none !important; 
}

/* 
remove all width constraints from content area
*/
div#content,
div#main {
display:block !important;
width:100% !important;
border:0 !important;
padding:1em !important;
}

/* hide everything else! */
div#header,
div#header h1 a,
div.breadcrumbs,
div#search,
div#footer,
div#menu_vert,
div#news,
div.right49,
div.left49,
div#sidebar  {
   display: none !important;
}

img {
float:none; /* this makes images couse a pagebreak if it doesnt fit on the page */
}
/* End of 'Siedevelop: Print' */

