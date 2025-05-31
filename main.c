/**
 * This is only a fun mini project from an online tutorial to learn c/c++.
 * You are free to make any chages to it.
 * Its an honor for me if you contribute to this project.
 * This is now compatible with linux only.
 */

// Including all necessary lbraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Definig global values for the shell
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_weather(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "weather"};

int (*builtin_func[])(char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit,
    &lsh_weather};

int lsh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

/*
  Builtin function implementations.
*/

/*
    Implementing cd command to change the directory in shell.
    We have used chdir function to do so.
 */
int lsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsg : Expected arguments to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lsh");
        }
    }
    return 1;
}
/**
 * It Shows help text to the user.
 */
int lsh_help(char **args)
{
    int i;
    printf("Kuunal Prajapti's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in :\n");
    for (i = 0; i < lsh_num_builtins(); i++)
    {
        printf(" %s\n", builtin_str[i]);
    }
    printf("Use the man command for information on other programs.");
    return 1;
}
/**
   With this we can exit the shell
 */
int lsh_exit(char **args)
{
    return 0;
}
/**
 * Weather function
 */
int lsh_weather(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected city name after 'weather'\n");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        char *url;
        size_t len = strlen("wttr.in/") + strlen(args[1]) + 1;
        url = malloc(len);
        if (!url)
        {
            fprintf(stderr, "lsh: memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        snprintf(url, len, "wttr.in/%s", args[1]);

        char *curl_args[] = {"curl", url, NULL};
        execvp("curl", curl_args);

        perror("lsh"); // if execvp fails
        free(url);
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Fork failed
        perror("lsh");
    }
    else
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }

    return 1;
}

/**
  @brief Launch a program and wait for it to terminate.
  @param args Null terminated list of arguments (including program).
  @return Always returns 1, to continue execution.
 */
int lsh_launch(char **args)
{
    pid_t pid, wpid;
    int status;
    pid = fork();
    if (pid == 0)
    {
        // child process
        if (execvp(args[0], args) == -1)
        {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // Error forking
        perror("lsh");
    }
    else
    {
        // Parent Process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}
/**
   @brief Execute shell built-in or launch program.
   @param args Null terminated list of arguments.
   @return 1 if the shell should continue running, 0 if it should terminate
 */
int lsh_execute(char **args)
{
    int i;
    if (args[0] == NULL)
    {
        // Empty Command was entered.
        return 1;
    }
    for (i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }
    return lsh_launch(args);
}

/**
   @brief Read a line of input from stdin.
   @return The line from stdin.
 */
char *lsh_read_line(void)
{
    char *line = NULL;
    size_t bufsize = 0; // have getline allocate a buffer for us

    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS); // We recieved an EOF
        }
        else
        {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}
/**
 * Tokenizing the input text to execute
 */
char **lsh_split_line(char *line)
{
    int bufsize = LSH_TOK_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "lsh: Allocation Error\n");
        exit(EXIT_FAILURE);
    }
    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                fprintf(stderr, "lsh: Allocation Error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}
/*
    Executing the shell
 */

void lsh_loop(void)
{
    char *line;
    char **args;
    int status;
    do
    {
        printf("K.P. SHELL >>> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        free(line);
        free(args);
    } while (status);
}

int main(int argc, char **argv)
{
    // Load Congfig file if any
    // Run loop Commands
    lsh_loop();
    // Perform any shutdown/cleanup.
    return EXIT_SUCCESS;
}