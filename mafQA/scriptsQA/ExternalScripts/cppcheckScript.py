import os
import sys
import getopt

currentPathScript = os.path.split(os.path.realpath(__file__))[0]
modulesDir = os.path.abspath(os.path.join(currentPathScript, "..", "..", ".."))

def usage():
    print "python cppcheckScript.py [-h] [-e cppcheckPath]"
    print "-h, --help                 show help (this)"
    #print "-e, --cppcheck-path=       cppcheck executable path"
    print 

def run(param):
    print "cppcheck working..."
    try:
        report = os.path.join(modulesDir, "report.xml")        
        cppcheck = param['cppcheckPath'] #find directly cppcheck absolute path
        command = cppcheck + " --enable=style --enable=information --enable=unusedFunction --xml " + modulesDir + " 2>" + report
        os.system(command)

    except Exception, e:
        print "Error on launching executable, ", e
        
        
        
def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "he", ["help", "cppcheck-path"])
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    param = {}
    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            return
        #elif o in ("-e", "--cppcheck-path"):
            #param = {'cppcheckPath': a}
        else:
            assert False, "unhandled option"

    import subprocess as sp
    result = sp.Popen(["which", "cppcheck"], stdout=sp.PIPE).communicate()[0]
    found = result.find("cppcheck")
    if(found == -1):
        print "cppcheck does not exist -", sys.argv[0]
        print os.environ
        print os.getuid()
        exit()

    param = {'cppcheckPath': result.replace("\n","")} #remove final \n
    run(param)


if __name__ == "__main__":
  main()