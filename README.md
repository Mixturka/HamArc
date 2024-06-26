# HamArc: Hamming Archive File Utility

HamArc is a robust file archiver designed to combine multiple files into a single `.haf` (Hamming Archive File) without compression, ensuring data integrity through Hamming error-correcting codes. This utility is capable of handling large files efficiently and is structured for optimal performance and maintainability.

## Features


- **Error Correction**: Utilizes Hamming codes to protect against data corruption.
- **Archiving**: Combines one or more files(or folders) into a single `.haf` archive.

- **Extraction**: Retrieves all or specific files from an archive.
- **Merging**: Combines multiple `.haf` archives into one.

- **File Management**: Adds or removes files from an existing archive.
- **Listing**: Displays the contents of an archive.

- **Recovery**: Attempts to repair corrupted archives or informs the user if recovery is not possible.

## Command line arguments
- ```-c, --create``` - Create new archive
- ```-f --file=FILE_NAME``` - Archive file name
- ```-l, --list``` - List all archived files
- ```-x, --extract``` - Extract file from archive (if no name provided extracts everything)
- ```-a, --append``` - Add file to archive
- ```-d, --delete``` - Delete file from archive
- ```-A, --concatenate``` - Merge two archives

## Usage

HamArc is a command-line application that supports various operations through command-line arguments. Here are some examples of how to use HamArc:


- **Creating an Archive**:
```bash
HamArc --create --file=ARCHIVE_NAME FILE1 FILE2 FILE3
```
- **Listing Files in an Archive**:
 ```bash
HamArc -l -f ARCHIVE_NAME
```
- **Merging Two Archives**:
```bash
HamArc --concatenate ARCHIVE1 ARCHIVE2 -f ARCHIVE3
```
