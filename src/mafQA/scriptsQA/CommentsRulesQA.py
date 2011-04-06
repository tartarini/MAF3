import os
import sys
import re
import math
import Executer
from Rules.CommentsRules import SourcesCommentRule

class CommentsRulesQA():
    def __init__(self):
        self.FileOutput = None
        self.OutputDirectory = None
        self.Classes = []
        self.NumberOfErroneusClasses = 0
        self.CurrentRule = ""
        self.FilePattern = ""

    def SetFilePattern(self, filePattern):
        self.FilePattern = filePattern

    def CheckClassFile(self, dirPath, file, rule):
        x = re.compile(self.FilePattern) 
        if(re.match(x, str(file))):
          executer = Executer.Executer()
          executer.setRule(rule)
          executer.setFileNameInput(str(file))
          executer.setFileNameOutput(self.CurrentRule + ".xml")
          executer.setDirectoryNameInput(str(dirPath))
          executer.setOutputDirectory(self.OutputDirectory)
          self.NumberOfErroneusClasses = self.NumberOfErroneusClasses + executer.parse()
          return file
        else:
          return None

    def SourcesCommentRule(self, dirPath, file):
        rule = SourcesCommentRule.SourcesCommentRule()
        rule.setRuleParameters(None)
        xmlFile = self.CheckClassFile(dirPath, file, rule)
        if(xmlFile != None):
          self.Classes.append(xmlFile)

    def CheckSources(self,baseDirectory, outputDirectoryCheck, ruleMethod):
        self.Classes = []
        self.CurrentRule = ruleMethod
        self.NumberOfErroneusClasses = 0
        self.OutputDirectory = outputDirectoryCheck
        try:
          os.remove(outputDirectoryCheck + self.CurrentRule +".xml")
        except:
          print "No file to remove"
        self.FileOutput = open(outputDirectoryCheck + self.CurrentRule +".xml",'a')
        self.FileOutput.write("<?xml version=\"1.0\"?>\n")
        self.FileOutput.write("<root>\n")
        self.FileOutput.write("<ruleName>\n")
        self.FileOutput.write(self.CurrentRule + "\n")
        self.FileOutput.write("</ruleName>\n")
        self.FileOutput.write("<ruleDescription>\n")
        self.FileOutput.write("Check that comments inside sources are present in a range from 20% up to 40%" + "\n")
        self.FileOutput.write("</ruleDescription>\n")
        self.FileOutput.write("<results>\n")
        self.FileOutput.close()
        for dirpath,dirnames,filenames in os.walk(baseDirectory):
          for file in filenames:
            getattr(self, self.CurrentRule)(dirpath, file)

        self.FileOutput = open(outputDirectoryCheck + self.CurrentRule +".xml",'a')
        percentage = int(float(len(self.Classes) - self.NumberOfErroneusClasses)/float(len(self.Classes)) * 100)
        percentageCoverage = int(float((len(self.Classes)- self.NumberOfErroneusClasses)/2)/(float((len(self.Classes)+ self.NumberOfErroneusClasses)/2)) * 100)
        # result XML tag open
        self.FileOutput.write("<percentage>\n")
        self.FileOutput.write("\n" + str(len(self.Classes)- self.NumberOfErroneusClasses) + " on " + str(len(self.Classes)) + " percentage: " + str(percentage) + "%")
        self.FileOutput.write("\n</percentage>\n")
        self.FileOutput.write("<percentageCoverage>\n")
        self.FileOutput.write("\n" + str((len(self.Classes)- self.NumberOfErroneusClasses)/2) + " on " + str((len(self.Classes)+ self.NumberOfErroneusClasses)/2) + " percentage: " + str(percentageCoverage) + "%")
        self.FileOutput.write("\n</percentageCoverage>\n")
        self.FileOutput.write("</results>\n")
        self.FileOutput.write("</root>\n")
        # result XML tag close
        self.FileOutput.close()

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print "need argv[1](directory to check), argv[2](directory in which generate scripts) "
    else:
        nc = CommentsRulesQA()
        nc.SetFilePattern(".*\.(cpp|txx|cxx)$")
        nc.CheckSources(sys.argv[1],  sys.argv[2],"SourcesCommentRule")
