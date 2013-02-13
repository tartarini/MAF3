import sys
import os
from AbstractPlugin import AbstractPlugin

currentPathScript = os.path.split(os.path.realpath(__file__))[0]

try:
    sys.path.append(os.path.realpath(os.path.join(currentPathScript,"..","..")))
    from qa import mafPath
except ImportError:
    import mafPath

class lcovPlugin(AbstractPlugin):
    def __init__(self):
        AbstractPlugin.__init__(self)
        self.currentModule = ""

    def execute(self):
        sourceDir = os.path.join(self.Parameters['sourceDir'])
        for item in os.listdir(sourceDir):
            if (os.path.isfile(os.path.join(sourceDir, item))==False):
                import re
                x = re.compile(self.Parameters['filter'])
                if(re.match(x, item)):
                   if(str(sys.platform).lower() == "win32"):
                       print "Script not valid on Windows"
                       exit()
               
                   disclaimer = """
                   ************************************************************
                   This Coverage Test has been developed using gcov/lcov suite.
                   It works only in unix-like systems and the code need to be builded
                   with these flags, for qmake
                   QMAKE_CXX_FLAGS_DEBUG += -fprofile-arcs
                   QMAKE_CXX_FLAGS_DEBUG += -ftest-coverage
                   QMAKE_LFLAGS_DEBUG += -fprofile-arcs
                   QMAKE_LFLAGS_DEBUG += -ftest-coverage
                   and for cmake:
                   find_program( CODECOV_GCOV gcov )
                   add_definitions( -fprofile-arcs -ftest-coverage )
                   link_libraries( gcov )
                   set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  --lgcov" )
                   set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  --lgcov" )
                   ************************************************************\n
                   """

                   #print disclaimer
                   #time.sleep(5)
    
                   import subprocess as sp
                   result = sp.Popen(["which", "lcov"], stdout=sp.PIPE).communicate()[0]
                   found = result.find("lcov")
                   if(found == -1):
                       print "lcov does not exist -", sys.argv[0]
                       print os.environ
                       print os.getuid()
                       exit()
                   
                   self.currentModule = item
                   self.run()

        
    def run(self):
        extScriptDir = currentPathScript
        baseDir = os.path.join(self.Parameters['sourceDir'])
        moduleDir = os.path.join(baseDir,self.currentModule)
        testDir = os.path.join(mafPath.mafTestsDir, self.currentModule + "Test")
        binDir = os.path.join(mafPath.mafBinaryDir, "bin") #here can be also with Debug
        qaResultsDir = os.path.join(mafPath.mafQADir, "QAResults")
        LCOVExternalCoverageDir = os.path.join(qaResultsDir, "externalCoverage")

        if(os.path.exists(baseDir) == False):
            print "Module Dir doesn't exist %s" % baseDir
            exit()

        if(os.path.exists(testDir) == False):
            print "Test Module Dir doesn't exist %s" % testDir
            exit()

        if(os.path.exists(binDir) == False):
            print "Binary Dir doesn't exist %s" % binDir
            exit()

        if(os.path.exists(qaResultsDir) == False):
            print "QA Results Dir doesn't exist %s" % qaResultsDir
            exit()

        if(os.path.exists(LCOVExternalCoverageDir) == False):
            os.mkdir(LCOVExternalCoverageDir);

        moduleCoverageReportDir = os.path.join(LCOVExternalCoverageDir, self.currentModule +"Coverage")

        os.system("rm -fR "+ moduleCoverageReportDir)
        os.mkdir(moduleCoverageReportDir);

        gcdaDir = os.path.join(mafPath.mafBinaryDir,"src",self.currentModule,"CMakeFiles",self.currentModule+".dir")
        
        try:
          os.chdir(gcdaDir)

          os.system("find . -type f -name '*.gcda' -print | xargs /bin/rm -f")

          executableTest = self.currentModule + "Test"
    
          os.chdir(binDir)
          os.environ['LD_LIBRARY_PATH'] = binDir
          os.environ['DISPLAY'] = "localhost:0.0"
          os.system("Xvfb :0.0 &")
          os.system("./" + executableTest)
    
          os.chdir(gcdaDir)
          os.system("find . -type f -name 'moc_*.gcno' -print | xargs /bin/rm -f")
          os.system("find . -type f -name 'moc_*.gcda' -print | xargs /bin/rm -f")
    
          os.system("find . -type f -name 'qrc_*.gcno' -print | xargs /bin/rm -f")
          os.system("find . -type f -name 'qrc_*.gcda' -print | xargs /bin/rm -f")
    
          os.system("find . -type f -name 'ui_*.gcno' -print | xargs /bin/rm -f")
          os.system("find . -type f -name 'ui_*.gcda' -print | xargs /bin/rm -f")

          commandLcov = "lcov  --directory . --capture --output-file " + moduleCoverageReportDir + "/" + self.currentModule + "_t.info"
          commandLcovExtract = "lcov  --extract " + moduleCoverageReportDir + "/" + self.currentModule + "_t.info \"*/"+self.currentModule+"*\" -o " + moduleCoverageReportDir + "/" + self.currentModule + "ext.info"
    
          commandLcovRemove = "lcov  --remove " + moduleCoverageReportDir + "/" + self.currentModule + "ext.info \"*/ui_*\" -o " + moduleCoverageReportDir + "/" + self.currentModule + ".info" 

          os.system(commandLcov)
          os.system(commandLcovExtract)
          os.system(commandLcovRemove)

          commandGenHtml = "genhtml -o " + moduleCoverageReportDir +  " --num-spaces 2 " + moduleCoverageReportDir + "/" + self.currentModule + ".info"

          os.system(commandGenHtml)
          os.chdir(extScriptDir)
        
        except:
          os.chdir(extScriptDir)
          
def usage():
    print "Usage:  python lcovPluginScript.py <moduleName>"

def execute():
    plugin = lcovPlugin
    plugin.execute()

def main():
    execute()

if __name__ == "__main__":
    main()
