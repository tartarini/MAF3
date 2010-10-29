import os
import sys

extensionToAvoid = [".dll",".prl", ".a",".lib", ".dylib",".so", ".0", ".1"] # necessary to insert .N because of symbolic link of linux

def find_executable(executable, path=None):
    fullPathName = path + "/" + executable
    if(os.access(fullPathName, os.X_OK)):
        ext = os.path.splitext(fullPathName)[-1]
        if(ext in  extensionToAvoid):
            return None
        else:
            return fullPathName
 
if __name__ == '__main__':
    scriptsDir = os.getcwd()
    os.chdir("../..")
    modulesDir = os.getcwd()
    dirList = [ name for name in os.listdir(modulesDir) if (name[0:3] == "maf" and name[3:] != "TestSuiteEngine" and os.path.isdir(os.path.join(modulesDir, name))) ] 
    print modulesDir, dirList
    execDir = modulesDir + "/../Install/bin/Debug"
    os.chdir(execDir)
    #some system requirements

    suffix = "Test"
    if(str(os.sys.platform).lower() == 'linux2'):
        #os.system('export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH')
        os.environ['LD_LIBRARY_PATH'] = os.environ['LD_LIBRARY_PATH'] + ":" + execDir
        suffix = "Test_debug"
        #need to start X
        os.environ['DISPLAY'] = "localhost:0.0"
        os.system("Xvfb :0.0 &")
    elif(str(os.sys.platform).lower() == 'darwin'):
        #os.system('export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH')
        os.environ['DYLD_LIBRARY_PATH'] = os.environ['DYLD_LIBRARY_PATH'] + ":" + execDir
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
