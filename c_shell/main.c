#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   // for execvp and fork
#include <sys/wait.h> // for wait

#define MAX_ARGS 64
#define DELIM " \t\r\n\a"

typedef struct
{
  char *name;
  char **args;
  char arg_count;
} Command;

// clang-format off
char *read_line(void);
int parse_line(char *line, Command *cmd);
void free_command (Command *cmd);
int execute_command(Command *cmd);
// clang-format on

int
main (int argc, char **argv)
{
  int status = 1;
  Command cmd;

  char *line = NULL;
  while (status)
  {
    printf ("x > ");		 // we print the prompt
    line = read_line ();	 // we read the line
    if (parse_line (line, &cmd)) // we parse the line
    {
      // we execute the command we parsed from the user
      status = execute_command (&cmd);
      free_command (&cmd);
    }
  }

  free (line);

  return 0;
}

char *
read_line (void)
{
  char *line = NULL;
  size_t bufsize = 0;

  if (getline (&line, &bufsize, stdin) == -1)
  {
    if (feof (stdin))
    {
      // End of file or Ctrl+D
      exit (0);
    }
    perror ("read_line");
    exit (1);
  }

  return line;
}

int
parse_line (char *line, Command *cmd)
{
  int bufsize = MAX_ARGS;
  int position = 0;
  char **tokens = malloc (bufsize * sizeof (char *));
  char *token;

  if (!token)
  {
    fprintf (stderr, "allocation error\n");
    exit (1);
  }

  token = strtok (line, DELIM);
  while (token != NULL)
  {
    tokens[position++] = token;
    if (position >= bufsize)
    {
      bufsize += MAX_ARGS;
      tokens = realloc (tokens, bufsize * sizeof (char *));
      if (!tokens)
      {
    fprintf (stderr, "allocation error\n");
    exit (1);
      }
    }

    token = strtok (NULL, DELIM);
  }

  tokens[position] = NULL;
  if (position == 0)
  {
    free (tokens);
    return 0;
  }

  cmd->args = tokens;
  cmd->arg_count = position;
  cmd->name = tokens[0];

  return 1;
}

void
free_command (Command *cmd)
{
  free (cmd->args);
}

int
execute_command (Command *cmd)
{
  pid_t pid;

  if (strcmp (cmd->name, "exit") == 0)
    return 0; // we exit our shell

  pid = fork (); // create a child process
  if (pid == -1)
  {
    fprintf (stderr, "fork() failed\n");
    exit (1);
  }

  if (pid == 0) // we are in the child process
    execvp (cmd->name, cmd->args);

  // we need to wait for the child process to finish
  // waitpid() cleans up the child process
  // otherwise it becomes a zombie!

  int status;
  do
  {
    (void) waitpid (pid, &status, WUNTRACED);
  } while (!WIFEXITED (status) && !WIFSIGNALED (status));

  return 1;
}