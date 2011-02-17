import sys
import os
import time
from datetime import datetime
import glob
import lxml.etree as etree
from lxml.etree import XSLT,fromstring

currentPathScript = os.path.split(os.path.realpath(__file__))[0]
modulesDir = os.path.abspath(os.path.join(currentPathScript, "..", "..", ".."))

def usage():
    print "Usage:  python cppcheckPublish.py"

def publishReport():
    extScriptDir = os.getcwd()
    os.chdir("../../..")
    baseDir = os.getcwd()
    qaResultsDir = os.path.join(baseDir,"QAResults")
    htmlDir = os.path.join(baseDir,"QAResults","html")
    xmlDir = os.path.join(baseDir,"QAResults","xml")
    reportFile = os.path.join(baseDir, "report.xml");
    
    try:
       filename = reportFile
       print "Formatting in HTML " + filename
       #with lxml parse the file
       f = open(filename)
       xml = fromstring(str(f.read()))

       #with lxml create html
       absPathXslt = os.path.join(currentPathScript,"cppcheck.xslt")
       
       fileXslt = open(absPathXslt, 'r') 
       #print xsltH + headString + centerString + str(fileXslt.read()) + tailString + xsltT
       xsl=  fromstring(fileXslt.read()) 
       style = XSLT(xsl)
       result = style.apply(xml)
           
       #print htmlDir + filename + ".html"
       html = open(os.path.join(baseDir, "report.html"), 'w')
       print >> html , style.tostring(result)
         
    except Exception, e:
        print "Problem when publishing cppcheck, error:" , e

    print "******* PUBLISH cppcheck report *********"
    os.chdir(extScriptDir) 

    

if __name__ == "__main__":
    publishReport()
