# RSA Algorithm Tool

This is an implementation of the RSA Algorithm in C. This project produces a CLI tool that can generate a pair of RSA keys and perform the encryption and decryption of files using those keys.

## How to compile

**Requirements**: You need to have *gcc* and *make* installed. You can install them by executing the following commands in a terminal:

```
  $ sudo apt update
  $ sudo apt install build-essential
```

Execute the following commands to download and compile the code:

```
  $ git clone https://github.com/iapost/rsa-tool
  $ cd rsa-tool
  $ make
```

The above produces an executable named *rsa*.

## How to run

To generate a public and a private key, use the following command:

```
  $ ./rsa -g
```

This will create in the current directory two files named *public.key* and *private.key* containing the two keys.

To encrypt a file use the following command:


```
  $ ./rsa -i <plaintext_file_path> -o <encrypted_file_path> -k <key_file_path> -e
```

To decrypt a file use the following command:


```
  $ ./rsa -i <encrypted_file_path> -o <plaintext_file_path> -k <key_file_path> -d
```

The available options are explained in the table below:

| Option    | Description                                        |
|-----------|----------------------------------------------------|
| -i <path> | Path to input file                                 |
| -o <path> | Path to output file                                |
| -k <path> | Path to key file                                   |
| -d        | Decrypt input and store results to output          |
| -e        | Encrypt input and store results to output          |
| -g        | Generates a pair of keys and saves them to 2 files |
| -h        | Shows this help message                            |

## License

Distributed under the GPL-3.0 License. See [`LICENSE`](LICENSE) for more information.
