<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="text"/>
  <xsl:template match="/">
    <xsl:for-each select="results/error">
<xsl:value-of select="@file"/>(<xsl:value-of select="@line"/>): (<xsl:value-of select="@severity"/>) <xsl:value-of select="@msg"/>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>
