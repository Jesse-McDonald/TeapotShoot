#Returns the paths of all files in a directory and all sub directories relative to start directory
import os
def tree(directory,target="f"):
	paths=[]
	for currentDir,dirs,files in os.walk(directory):
		if target=="f":
			for file in files:
				paths.append(currentDir+"/"+file)
			for dir in dirs:
				paths+=(tree(dir))
		if target=="d":
			paths.append(currentDir)
			for dir in dirs:
				paths+=(tree(dir,"d"))
	return paths




