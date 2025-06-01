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
#include <time.h>

// Definig global values for the shell
#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
#define TODO_FILE "todo.txt" //Location for todo file to save in system

/*
  Function Declarations for builtin shell commands:
 */
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_weather(char **args);
int lsh_radio(char **args);
int lsh_todo(char **args);

/*
  List of builtin commands, followed by their corresponding functions.
 */
char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "weather",
    "radio",
    "todo"};

int (*builtin_func[])(char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit,
    &lsh_weather,
    &lsh_radio,
    &lsh_todo};

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
 * This function uses wttr.in api to get weather data
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
 * Function to play music on the terminal
 * Following pagages are neede to use it
 *  sudo apt install mpv yt-dlp
 */
int lsh_radio(char **args)
{
    // I have added these five links only because I have found only these working if you find any other radio station working then add that in these and help to make this shell more powerfull and a fun project
    const char *stations[] = {
        "http://air.pc.cdn.bitgravity.com/air/live/pbaudio048/playlist.m3u8", // AIR FM Rainbow
        "http://stream.radiobollyfm.in:8201/",
        "http://eu8.fastcast4u.com:5484/1/",
        "http://www.streamcontrol.net:12010/",
        "http://mehefil.no-ip.com/"};
    const char *names[] = {
        "AIR FM Rainbow",
        "Radio Bolly FM",
        "Akash Vani Radio",
        "Bolly 92.3 FM",
        "Mehefil Radio"};
    const int num_stations = sizeof(stations) / sizeof(stations[0]);
    printf("Available Radio Stations:\n\n");
    for (int i = 0; i < num_stations; i++)
    {
        printf("  %d. %s\n", i + 1, names[i]);
    }

    printf("\nSelect a station to play [1-%d]: ", num_stations);
    int choice = 0;
    scanf("%d", &choice);

    if (choice < 1 || choice > num_stations)
    {
        printf("Invalid choice.\n");
        return 1;
    }

    printf("\n Playing: %s\n", names[choice - 1]);
    printf(" Press Ctrl+C to stop listening at any time.\n\n");

    // Build the playback command using mpv
    char command[512];
    snprintf(command, sizeof(command), "mpv --no-video \"%s\"", stations[choice - 1]);
    system(command);

    return 1;
}

/**
 * Todo functions 
 */

 //Function t access todo file
char *get_todo_file_path() {
    static char path[1024];
    snprintf(path, sizeof(path), "%s/%s", getenv("HOME"), TODO_FILE);
    return path;
}
// Function to add task
void todo_add(char *task, char *priority, char *deadline) {
    FILE *file = fopen(get_todo_file_path(), "a");
    if (!file) {
        perror("Error opening todo file");
        return;
    }
    fprintf(file, "%s|%s|%s\n", task, priority, deadline);
    fclose(file);
    printf("Task added: %s (Priority: %s, Deadline: %s)\n", task, priority, deadline);
}
//Function to View all the task
void todo_list() {
    FILE *file = fopen(get_todo_file_path(), "r");
    if (!file) {
        printf(" No tasks found.\n");
        return;
    }
    char line[1024];
    int id = 1;

    printf("\n Your To-Do List:\n");
    printf("------------------------------------------------------------\n");
    printf("ID | Task                        | Priority | Deadline\n");
    printf("------------------------------------------------------------\n");
    
    while (fgets(line, sizeof(line), file)) {
        char *task = strtok(line, "|");
        char *priority = strtok(NULL, "|");
        char *deadline = strtok(NULL, "|\n");
        
        printf("%-3d| %-27s| %-8s| %s\n", id++, task, priority, deadline);
    }
    
    printf("------------------------------------------------------------\n");
    fclose(file);
}
//Function to mark a task to done
void todo_done(int task_id) {
    FILE *file = fopen(get_todo_file_path(), "r");
    if (!file) {
        printf(" No tasks to complete.\n");
        return;
    }

    FILE *temp = fopen("/tmp/todo_tmp.txt", "w");
    if (!temp) {
        perror("Temp file error");
        fclose(file);
        return;
    }

    char line[1024];
    int id = 1;
    int removed = 0;

    while (fgets(line, sizeof(line), file)) {
        if (id != task_id) {
            fputs(line, temp);
        } else {
            removed = 1;
        }
        id++;
    }

    fclose(file);
    fclose(temp);

    if (removed) {
        rename("/tmp/todo_tmp.txt", get_todo_file_path());
        printf("Task %d marked as done!\n", task_id);
    } else {
        remove("/tmp/todo_tmp.txt");
        printf("Task not found.\n");
    }
}
//Todo command help function
void todo_help() {
    printf("To-Do Command Help\n");
    printf("--------------------------------------------------\n");
    printf("Usage:\n");
    printf("  todo add <task> [-p priority] [-d deadline]\n");
    printf("     Add a task with optional priority and deadline.\n\n");
    printf("  todo list\n");
    printf("     List all tasks.\n\n");
    printf("  todo done <task_id>\n");
    printf("     Mark a task as completed.\n\n");
    printf("  todo help\n");
    printf("     Show this help message.\n");
    printf("--------------------------------------------------\n");
}

/**
 * Todo actual function
 */
int lsh_todo(char **args) 
{
        if (args[1] == NULL) {
        todo_help();
        return 1;
    }

    if (strcmp(args[1], "help") == 0) {
        todo_help();
    } else if (strcmp(args[1], "add") == 0) {
        char task[512] = "";
        char priority[32] = "Normal";
        char deadline[32] = "None";

        int i = 2;
        while (args[i] != NULL && args[i][0] != '-') {
            strcat(task, args[i]);
            strcat(task, " ");
            i++;
        }

        while (args[i] != NULL) {
            if (strcmp(args[i], "-p") == 0 && args[i + 1]) {
                strcpy(priority, args[++i]);
            } else if (strcmp(args[i], "-d") == 0 && args[i + 1]) {
                strcpy(deadline, args[++i]);
            }
            i++;
        }

        todo_add(task, priority, deadline);
    } else if (strcmp(args[1], "list") == 0) {
        todo_list();
    } else if (strcmp(args[1], "done") == 0 && args[2]) {
        int id = atoi(args[2]);
        if (id > 0) {
            todo_done(id);
        } else {
            printf("Invalid task ID.\n");
        }
    } else {
        todo_help();
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