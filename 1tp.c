#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  void usage() {

    fprintf(stderr, "usage: %s one_time_pad\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "This program will encrypt and decrypt data streams via one-time-pad.  It takes input from stdin and outputs to stdout.  A one-time-pad file must be supplied as the first and only argument.  This file will be consumed \"head-first\" with each usage, by the amount of bytes encrypted or decrypted.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Example usage:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "First, generate a one-time-pad:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "dd if=/dev/random of=one-time-pad count=1024    ;Store random bytes to file.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Give a single copy to a friend (do not send over wire; be mindfull of every location these bytes reside on disk).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Next, encrypt a file:\n");
    fprintf(stderr, "(make sure there is enough data left in the one-time-pad; this operation will consume [size of input] bytes of one-time-pad)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "cat some-input-file.any-type | %s one-time-pad > output-file\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "Send this file however you please; it's contents are mathematically proven to be 100%% cryptographically secure, assuming the one-time-pad was truly random and kept physically secure.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "Lastly, have your friend decrypt the file:\n");
    fprintf(stderr, "(this operation will consume [size of input] bytes of one-time-pad)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "cat output-file | %s one-time-pad > some-input-file.any-type\n", argv[0]);
    fprintf(stderr, "\n");
    fprintf(stderr, "Considerations:\n");
    fprintf(stderr, "Size of data is not encrypted.\n");
    fprintf(stderr, "This work is dedicated to the memory of Claude Elwood Shannon.\n");
    fprintf(stderr, "Special thanks to Gilbert Sandford Vernam.\n");

  }

  // validate number of arguments passed
  if (argc != 2) {
    usage();
    return 0;
  }

  // one-time-pad file descriptors
  FILE * padFile;
  FILE * padFileShrink;

  // variables to hold current bytes of both input and one-time-pad
  int input, pad;

  // open file passed as first argument
  padFile = fopen(argv[1],"r");

  // proceed if successful
  if (padFile != NULL) {

    // assign byte from stdin to input variable, breaking at end-of-file
    while ((input = fgetc(stdin)) != EOF) {

      // assign byte from one-time-pad to pad variable, removing file on end-of-file
      if ((pad = fgetc(padFile)) == EOF) {

        // close one-time-pad file
        fclose(padFile);

        // remove one-time-pad file
        remove(argv[1]);

        // return exit code of zero
        return 0;

      }

      // XOR input byte with pad byte, write resulting byte to stdout
      fputc(input ^ pad, stdout);

    }

    // open file passed as first argument
    padFileShrink = fopen(argv[1],"r+b");

    // proceed if successful
    if (padFileShrink != NULL) {

      // assign byte from remaining one-time-pad to pad variable, breaking at end-of-file
      while ((pad = fgetc(padFile)) != EOF) {

        // assign pad variable start of truncated one time pad
        fputc(pad, padFileShrink);

      }

      // truncate one-time-pad
      ftruncate(fileno(padFileShrink), ftell(padFileShrink));

          // close one-time-pad file
          fclose(padFileShrink);

    }

    // close one-time-pad file
    fclose(padFile);

    // return exit code of zero
    return 0;

  } else usage();

    // return exit code of zero
    return 0;

}
