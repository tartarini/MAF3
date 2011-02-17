<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:str="http://exslt.org/strings" extension-element-prefixes="str">
<xsl:output method="html" encoding="UTF-8"/>
<xsl:template match="/">
<html>
<!-- **** layout stylesheet **** -->
<link rel="stylesheet" type="text/css" href="Styles/style.css" />

<!-- **** colour scheme stylesheet **** -->
<link rel="stylesheet" type="text/css" href="Styles/colour.css" />
<head><title>Books</title>
</head>
<body>
<table width="100%" border="1">
  <THEAD>
	<TR>
	   <TD width="30%"><B>File</B></TD>
 	   <TD width="15%"><B>id</B></TD>
 	   <TD width="15%"><B>line</B></TD>
	   <TD width="10%"><B>severity</B></TD>
	   <TD width="15%"><B>msg</B></TD>
	</TR>
  </THEAD> 
  <TBODY>
	<xsl:for-each select="results/error">
	<xsl:variable name="nodeset" select="str:tokenize(@file, '/')"/>
	<xsl:choose>
       <xsl:when test="position() mod 2 != 1">
        <tr class="odd">
        <TD width="30%"><xsl:value-of select="$nodeset[count($nodeset)]" /></TD>  	
	   <TD width="15%"><xsl:value-of select="@id" /></TD>
	   <TD width="15%"><xsl:value-of select="@line" /></TD> 
   	   <TD width="10%"><xsl:value-of select="@severity" /></TD>
	   <TD width="15%"><xsl:value-of select="@msg" /></TD>
        </tr>
       </xsl:when>
       <xsl:otherwise>
        <tr class="even">
        <TD width="30%"><xsl:value-of select="$nodeset[count($nodeset)]" /></TD>  	
	   <TD width="15%"><xsl:value-of select="@id" /></TD>
	   <TD width="15%"><xsl:value-of select="@line" /></TD> 
   	   <TD width="10%"><xsl:value-of select="@severity" /></TD>
	   <TD width="15%"><xsl:value-of select="@msg" /></TD>
        </tr>
       </xsl:otherwise>
     </xsl:choose>
    
	</xsl:for-each>
  </TBODY>
</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>