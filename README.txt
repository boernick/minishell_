***MAKEFILE***

TO GET ALL THE SRCS *.C FILEPATHS FOR YOUR MAKEFILE: GO TO THE SRCS FOLDER IN TERMINAL AND TYPE:

(find . -type f -name "*.c" | sed 's|^\./|srcs/|') | xclip -selection clipboard

PASTE CLIPBOARD IN MAKEFILE.


***GIT***

CREATE REPOSITORY
...
// STARTING
	git pull
	git status //TO SHOW WHAT BRANCH YOU ARE ON
	git checkout -b <BRANCH_NAME>
	code .
// DO SOME CODING
	git status //TO SHOW WHAT BRANCH YOU ARE ON
	git add .
	git commit -m "custom message"
	git push -u origin <BRANCH_NAME>
// GO BACK TO MASTER
	git checkout master
	git pull

**DEBUG VALGRIND**
valgrind --trace-children=yes --track-fds=yes ./pipex_new in.txt cat cat out.txt


***BASH TESTING REDIRECTIONS****
find /etc type -f 1> results.txt 2> ~/errors.txt;
	//this example will find all filetypes including /etc, and redirect standard error to results.txts, and redirect stderror to errors.txt
find /etc type -f > results.txt 2> ~/errors.txt;
	// this works the same way: the files will be completely wiped out and overwritten with the > single bracket
	// 							to append the existing files you can use the double brackets >> instead


testing:
echo "Hello World" > hallo.txt
echo "Hello World" >> hallo.txt

