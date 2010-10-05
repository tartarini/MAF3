import sys
import os
#import time

currentModule = ""

def usage():
    print "Usage:  python LCOVCoverageScript.py <moduleName>"

def createCoverageReport():
    extScriptDir = os.getcwd()
    os.chdir("../../..")
    baseDir = os.getcwd()
    moduleDir = baseDir + "/" + currentModule
    testDir = moduleDir + "/" + currentModule + "Test"
    binDir = baseDir + "/../Install/bin/Debug"
    qaResultsDir = baseDir + "/QAResults"
    LCOVExternalCoverageDir = qaResultsDir + "/externalLCOVCoverage" 

    if(os.path.exists(moduleDir) == False):
        print "Module Dir doesn't exist %s" % moduleDir
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

    moduleCoverageReportDir = LCOVExternalCoverageDir + "/" + currentModule +"Coverage" 
        

    os.system("rm -fR "+ moduleCoverageReportDir)
    os.mkdir(moduleCoverageReportDir);

    os.chdir(moduleDir)
    os.system("find . -type f -name '*.gcda' -print | xargs /bin/rm -f")

    executableTest = currentModule + "Test_debug"
    
    os.chdir(binDir)
    os.environ['LD_LIBRARY_PATH'] = binDir
    os.environ['DISPLAY'] = "localhost:0.0"
    os.system("Xvfb :0.0 &")
    os.system("./" + executableTest)

    os.chdir(moduleDir)
    os.system("find . -type f -name 'moc_*.gcno' -print | xargs /bin/rm -f")
    os.system("find . -type f -name 'moc_*.gcda' -print | xargs /bin/rm -f")

    dirList = os.listdir(moduleDir)
    for d in dirList:
        fullPathD = moduleDir + "/" + d
        if os.path.isdir(fullPathD) == True:
            os.chdir(fullPathD)
            os.system("find . -type f -name '*.gcno' -print | xargs /bin/rm -f")
            os.system("find . -type f -name '*.gcda' -print | xargs /bin/rm -f")


    
    os.chdir(moduleDir) 

    commandLcov = "lcov  --directory . --capture --output-file " + moduleCoverageReportDir + "/" + currentModule + "_t.info"
    commandLcovExtract = "lcov  --extract " + moduleCoverageReportDir + "/" + currentModule + "_t.info \"*/maf*\"  -o " + moduleCoverageReportDir + "/" + currentModule + ".info" 

    os.system(commandLcov)
    os.system(commandLcovExtract)

    commandGenHtml = "genhtml -o " + moduleCoverageReportDir +  " --num-spaces 2 " + moduleCoverageReportDir + "/" + currentModule + ".info"

    os.system(commandGenHtml)
    os.chdir(extScriptDir)

if __name__ == "__main__":
    if(str(sys.platform).lower() == "win32"):
        print "Script not valid on Windows"
        exit()

    disclaimer = """
    ************************************************************
    This Coverage Test has been developed using gcov/lcov suite.
    It works only in unix-like systems and the code need to be builded
    with these flags:
    QMAKE_CXXFLAGS_DEBUG += -fprofile-arcs
    QMAKE_CXXFLAGS_DEBUG += -ftest-coverage
    QMAKE_LFLAGS_DEBUG += -fprofile-arcs
    QMAKE_LFLAGS_DEBUG += -ftest-coverage
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

    if(len(sys.argv) != 2):
        usage()
        exit()

    currentModule = sys.argv[1]
    createCoverageReport()
