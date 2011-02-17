<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns:str="http://exslt.org/strings" extension-element-prefixes="str">
<xsl:output method="html" encoding="UTF-8"/>
<xsl:template match="/">
<html>
<!-- **** layout stylesheet **** -->
<link rel="stylesheet" type="text/css" href="mafQA/ScriptsQA/Styles/style.css" />

<!-- **** colour scheme stylesheet **** -->
<link rel="stylesheet" type="text/css" href="mafQA/ScriptsQA/Styles/colour.css" />
<head><title>cppcheck Report</title>
</head>
<body>
<div id="main">
    <div id="links">
      <!-- **** INSERT LINKS HERE **** -->
      <!--a href="#">another link</a> | <a href="#">another link</a> | <a href="#">another link</a> | <a href="#">another link</a-->
    @@@_PUBLISH_DATE_@@@
    </div>
    <div id="logo"><h1>MAF Quality Assurance</h1></div>
    <div id="content">
      <div id="menu">
        <ul>
          <li><a href="index.html">Introduction</a></li>
          <!-- **** INSERT ADDITIONAL EXTERNAL TOOLS REPORT_LINK HERE **** -->
          @@@_EXTERNAL_TOOLS_REPORT_@@@
        </ul>
      </div>


<div id="contentTable" align="center">
 <table id="listing">
  <thead>
	<tr>
	   <th width="30%" align="center"><b>File</b></th>
 	   <th width="10%" align="center"><b>Type</b></th>
 	   <th width="10%" align="center"><b>Line</b></th>
	   <th width="15%" align="center"><b>Severity</b></th>
	   <th width="35%" align="center"><b>Message</b></th>
	</tr>
  </thead> 
  <tbody>
	<xsl:for-each select="results/error">
	<xsl:variable name="nodeset" select="str:tokenize(@file, '/')"/>
	<xsl:choose>
       <xsl:when test="position() mod 2 != 1">
        <tr class="odd">
        <td width="30%"><xsl:value-of select="$nodeset[count($nodeset)]" /></td>  	
	   <td width="10%" align="center"><xsl:value-of select="@id" /></td>
	   <td width="10%" align="center"><xsl:value-of select="@line" /></td> 
   	   <td width="15%" align="center"><xsl:value-of select="@severity" /></td>
	   <td width="35%"><xsl:value-of select="@msg" /></td>
        </tr>
       </xsl:when>
       <xsl:otherwise>
        <tr class="even">
        <td width="30%"><xsl:value-of select="$nodeset[count($nodeset)]" /></td>  	
	   <td width="10%" align="center"><xsl:value-of select="@id" /></td>
	   <td width="10%" align="center"><xsl:value-of select="@line" /></td> 
   	   <td width="15%" align="center"><xsl:value-of select="@severity" /></td>
	   <td width="35%"><xsl:value-of select="@msg" /></td>
        </tr>
       </xsl:otherwise>
     </xsl:choose>
    
	</xsl:for-each>
  </tbody>
 </table>
</div>
</div>
</div>
</body>
</html>
</xsl:template>
</xsl:stylesheet>