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

class ccccPlugin(AbstractPlugin):
    def __init__(self):
        AbstractPlugin.__init__(self)

    def execute(self):
        import subprocess as sp
        result = sp.Popen(["which", "cccc"], stdout=sp.PIPE).communicate()[0]
        found = result.find("cccc")
        if(found == -1):
            print "cccc does not exist -", sys.argv[0]
            print os.environ
            print os.getuid()
            exit()
        
        if(not self.Parameters.has_key('ccccPath')):
            self.Parameters['ccccPath'] = result.replace("\n","") #remove final \n
            self.run()
        
    def run(self):
        print "cccc working..."
        try:
            sourceDir = os.path.join(self.Parameters['sourceDir'])
            qaResultsDir = os.path.join(qaDir, "QAResults")
            CCCCDir = os.path.join(qaResultsDir, "externalcccc") #need to parametrize this?
        
            if(os.path.exists(qaResultsDir) == False):
              print "QA Results Dir doesn't exist %s" % qaResultsDir
              exit()

            if(os.path.exists(CCCCDir) == False):
              os.mkdir(CCCCDir);
            
            import re
            x = re.compile(self.Parameters['filter'])
            for item in os.listdir(sourceDir):
                moduleDir = os.path.join(sourceDir , item)
                if (os.path.isfile(moduleDir)==False):
                    if(re.match(x, item)):
                        reportXML = os.path.join(CCCCDir, item + "cccc.xml")     
                        cccc = self.Parameters['ccccPath'] #find directly cppcheck absolute path
                        command = cccc + " --xml_outfile=" + reportXML + " " + moduleDir + "/*.cpp"
                        #print command
                        os.system(command)

        except Exception, e:
            print "Error on launching executable, ", e

def usage():
    print "python ccccPluginScript.py [-h] [-e ccccPath]"
    print "-h, --help                 show help (this)"
    #print "-e, --cccc-path=       cccc executable path"
    print 
            
def execute():
    plugin = ccccPlugin    
    plugin.execute()

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hm:e", ["help", "module", "cccc-path"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
        
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            return
        elif o in ("-m", "--module="):
            param['module'] = a
        else:
            assert False, "unhandled option"
    execute()


if __name__ == "__main__":
    main()
