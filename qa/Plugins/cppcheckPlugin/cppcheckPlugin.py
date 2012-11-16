import os
import sys
import getopt
from AbstractPlugin import AbstractPlugin

currentPathScript = os.path.split(os.path.realpath(__file__))[0]

try:
    sys.path.append(os.path.realpath(os.path.join(currentPathScript,"..","..")))
    from qa import mafPath
except ImportError:
    import mafPath

modulesDir = mafPath.mafSourcesDir
qaDir = mafPath.mafQADir

class cppcheckPlugin(AbstractPlugin):
    def __init__(self):
        AbstractPlugin.__init__(self)

    def execute(self):
        import subprocess as sp
        result = sp.Popen(["which", "cppcheck"], stdout=sp.PIPE).communicate()[0]
        found = result.find("cppcheck")
        if(found == -1):
            print "cppcheck does not exist -", sys.argv[0]
            print os.environ
            print os.getuid()
            exit()

        self.Parameters['cppcheckPath'] = result.replace("\n","") #remove final \n
        self.run()

        
    def run(self):
        print "cppcheck working..."
        try:
            baseDir = modulesDir
            qaResultsDir = os.path.join(qaDir, "QAResults")
            CPPCheckStaticAnalisysDir = os.path.join(qaResultsDir, "externalcppcheck") #need to parametrize this?
        
            if(os.path.exists(qaResultsDir) == False):
              print "QA Results Dir doesn't exist %s" % qaResultsDir
              exit()

            if(os.path.exists(CPPCheckStaticAnalisysDir) == False):
              os.mkdir(CPPCheckStaticAnalisysDir);
        
            report = os.path.join(CPPCheckStaticAnalisysDir, "report.xml")        
            cppcheck = self.Parameters['cppcheckPath'] #find directly cppcheck absolute path
            command = cppcheck + " --enable=style --xml " + modulesDir + " 2>" + report
            # --enable=information --enable=unusedFunction
            os.system(command)

        except Exception, e:
            print "Error on launching executable, ", e


def usage():
    print "python cppcheckPluginScript.py [-h] [-e cppcheckPath]"
    print "-h, --help                 show help (this)"
    #print "-e, --cppcheck-path=       cppcheck executable path"
    print 
        
def execute():
    plugin = cppcheckPlugin
    plugin.execute()
        
def main():
    print "Executing cppcheck..."
    return
    try:
        opts, args = getopt.getopt(sys.argv[1:], "he", ["help", "cppcheck-path"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            return
        #elif o in ("-e", "--cppcheck-path"):
            #param = {'cppcheckPath': a}
        else:
            assert False, "unhandled option"
    execute()

if __name__ == "__main__":
    main()
