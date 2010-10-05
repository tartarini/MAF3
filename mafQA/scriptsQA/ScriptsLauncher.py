import os
import sys
import getopt


def run(param):
    scriptsDir = os.getcwd()
    f = None
    try:
        f = open("TestListQA.txt")
    except:
        print "QA FAILED"
        print "Probem open TestListQA.txt"

    lines = f.readlines()
    python = "python "

    suffix = "QA.py"
    fileSuffix = "FilePattern.ini"

    resultDir = "../../QAResults/xml/"
    if not os.path.exists(resultDir):
        os.makedirs(resultDir)

    for line in lines:
        line = line.replace("\r", "").replace("\n", "")
        ruleGroup = line.replace(suffix,"")

        print "QA Running...", ruleGroup

        filePattern = ruleGroup + fileSuffix
  
        r = open("./Rules/" + filePattern)
        linesRule = r.readlines()
        #  print linesRule
  
        sourceDir = str(linesRule[1]).rsplit("=")[1]
  
        command = python + line + " " + sourceDir + " " + resultDir
        os.system(command.replace("\"","").replace("\r", "").replace("\n", ""))

    print "QA SUCCESSFUL"


    if(param['LCOVCoverage']):
        os.chdir("../../")
        baseDir = os.getcwd()
        externalScriptDirectory = scriptsDir + "/ExternalScripts"

        for item in os.listdir(baseDir):
            if (os.path.isfile(os.path.join(baseDir, item))==False):
                if(item.find("maf") != -1):
                    os.chdir(externalScriptDirectory)
                    os.system("python " + externalScriptDirectory + "/LCOVCoverageScript.py "+ item)

    os.chdir(scriptsDir)
          
def usage():
    print "Usage: python ScriptLauncher.py [--enable-LCOVCoverage]"

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "l", ["enable-LCOVCoverage",])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    LCOVCoverageFlag = False
    for o, a in opts:
        if o in ("-l", "--enable-LCOVCoverage"):
            LCOVCoverageFlag = True
        else:
            assert False, "unhandled option"

    param = {'LCOVCoverage':LCOVCoverageFlag}
    run(param)
    
if __name__ == "__main__":
  main()
