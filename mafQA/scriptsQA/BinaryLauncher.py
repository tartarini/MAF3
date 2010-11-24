import os
import sys
import getopt

extensionToAvoid = [".dll",".prl", ".a",".lib", ".dylib",".so", ".0", ".1"] # necessary to insert .N because of symbolic link of linux
currentPathScript = os.path.split(os.path.realpath(__file__))[0]
param = {}

def find_executable(executable, path=None):
    fullPathName = path + "/" + executable
    if(os.access(fullPathName, os.X_OK)):
        ext = os.path.splitext(fullPathName)[-1]
        if(ext in  extensionToAvoid):
            return None
        else:
            return fullPathName

def execute():
    scriptsDir = currentPathScript
    modulesDir = os.path.join(currentPathScript, "..", "..")
    dirList = [ name for name in os.listdir(modulesDir) if (name[0:3] == "maf" and os.path.isdir(os.path.join(modulesDir, name))) ] 
    print modulesDir, dirList
    #execDir = modulesDir + "/../Install/bin/Debug"
    execDir = param['directory']
    os.chdir(execDir)
    #some system requirements

    suffix = "Test"
    if(str(os.sys.platform).lower() == 'linux2'):
        #os.system('export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH')
        try:
            os.environ['LD_LIBRARY_PATH'] = os.environ['LD_LIBRARY_PATH'] + ":" + execDir
        except KeyError:
            os.environ['LD_LIBRARY_PATH'] = execDir
            
        suffix = "Test_debug"
        #need to start X
        os.environ['DISPLAY'] = "localhost:0.0"
        os.system("Xvfb :0.0 &")
    elif(str(os.sys.platform).lower() == 'darwin'):
        #os.system('export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH')
        try:
            os.environ['DYLD_LIBRARY_PATH'] = os.environ['DYLD_LIBRARY_PATH'] + ":" + execDir
        except KeyError:
            os.environ['DYLD_LIBRARY_PATH'] = execDir
        
        suffix = "Test_debug"
    elif(str(os.sys.platform).lower() == 'win32'):
        suffix = "Test_d"
        

    for dir in dirList:
        executable = dir  + suffix
        if(os.path.exists(executable) or os.path.exists(executable + ".exe")):
            #print os.getcwd() + "/" + executable
            os.system(execDir + "/" + executable)
        else:
            print "Executable %s Not Present in %s" % (executable , execDir)
    os.chdir(scriptsDir)

def usage():
    print "python BinaryLauncher.py -d directory"

def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "d:", ["directory="])
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        usage()
        sys.exit(2)
    
    for o, a in opts:
        if o in ("-d", "--directory"):
            param['directory'] = os.path.abspath(os.path.normpath(a))
        else:
            assert False, "unhandled option"
    
    if(len(param) == 0):
        usage()
        #print currentPathScript
        return
    
    execute()
    
    
if __name__ == '__main__':
    main()
