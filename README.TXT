echo "hello" | grep "hello" > outfile
valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --suppressions=suppressions.supp --track-fds=yes ./minishell

to trace FDs
valgrind --trace-children=yes --suppressions=suppressions.supp --track-fds=yes ./minishell
