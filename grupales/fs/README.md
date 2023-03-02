# fisopfs

Sistema de archivos tipo FUSE.

## Operaciones Requeridas

Se pide en la consigna que se implemente el soporte a las siguientes funcionalidades:

- [`touch`](#touch)
- [`mkdir`](#mkdir)
- [`ls`](#ls)
- [`cat`](#cat)
- [`more`](#more)
- [`less`](#less)
- [`stat`](#stat)
- [`rm`](#rm)
- [`rmdir`](#rmdir)
- [`write`](#write)

Las cuales realizan las llamadas al sistema que debemos implementar. Las firmas de las mismas están definidas en [`<fuse.h>`](https://libfuse.github.io/doxygen/structfuse__operations.html).

## Representación del Sistema de Archivos

Nuestro file system será una estructura compuesta por dos vectores de datos diferentes. Los `dentries` y los `inodes`. Los `dentries` representarán los directorios del filesystem mientras que los `inodes` representarán a los archivos. Cada tipo de dato contendrá además metadata necesaria de cada elemento.

### INodes

Los `inodes` representarán a los archivos. Cada `inode` contendrá la siguiente información:

```c
struct inode {
    int inumber;
    int fileSize;
    char type;
    char name[256];
    char* data;
    int timesOpen;
    long int birth;
    long int mod;
    struct dentry* parent;
};
```

Su nombre, número de inodo, tamaño, directorio padre, tipo (utilizado o no utilizado) y un puntero a los datos que contiene.

### Directory Entries

Los `dentries` representarán a los directorios. Cada `dentry` contendrá la siguiente información:

```c
struct dentry {
    int numberOfInodes;
    int timesOpen;
    char name[256];
    struct inode (*inodes)[10];
};
```

Su nombre, peso total y referencia a una tabla de `inodes` que posee los archivos que contiene.

### File System

El filesystem contendrá una referencia a los archivos en root (en forma de un puntero a un array de inodos) y una referencia a los directorios (en forma de un puntero a un array de dentries).

```c
struct filesystem {
    int numberOfInodes;
    int numberOfDentries;
    int size;
    struct dentry (*dentries)[MAX_ROOT_DIR];     // sizeof(MAX_ROOT_DIR)
    struct inode (*root_inodes)[MAX_ROOT_FILES]; // sizeof(MAX_ROOT_FILES) 
};
```

También tenemos una referencia a su peso. 

## Tamaño de disco máximo

En cada operación donde aumente el `filesystem->size` tenemos que verificar que este no supere un máximo predefinido. Suponiendo que `sizeof(struct filesystem) = 32`, `sizeof(struct inode) = 288` y `sizeof(struct dentry) = 272`, con `MAX_ROOT_DIR = MAX_ROOT_FILES = 100` y `MAX_FILES_IN_DIR = 10` tenemos que el tamaño máximo del `struct filesystem` es de $32 + 288 \cdot 100 + (272 + 288 \cdot 10) \cdot 100 + = 344032$ bytes.

Por lo tanto, como podemos tener un máximo de 100 archivos en root y 10 archivos en cada directorio, la cantidad máxima de archivos será de $100 + 100 \cdot 10 = 1100$. Asumiendo un tamaño promedio de 1MB por archivo, el tamaño máximo del disco será de $1100 \cdot 1MB = 1.0742 GB$ (sin contar los $344032$ bytes requeridos para el filesystem). Esto nos ayudará a evitar la creación de archivos enormes.

## Búsqueda de Archivos

Para poder buscar archivos en el filesystem, se implementará una función que lea el string y lo detecte uno de los dos formatos:

```
./file
./dir/file
```

Donde el primero representa un archivo en root (por lo cual debemos hacer una búsqueda en `filesystem->root_inodes`) y el segundo representa un archivo en un directorio (por lo cual debemos hacer una búsqueda en el array de `filesystem->dentries` del valor `dir` y luego en este, hacer una búsqueda de `file` en el array de `dentry->inodes`).

## Serialización

El archivo de serialización tendrá el siguiente formato. Los primeros 12 bytes corresponden a los atributos del filesystem.

```c
    int numberOfInodes;
    int numberOfDentries;
    int size;
```

Luego los `numberOfInodes * sizeof(struct inode)` bytes (junto con la data) correspondientes a los inodos que vivan en root mientras que luego tendremos `numberOfDentries` entradas de directorios diferentes. Para saber cuantos bytes ocupan cada uno de estos dentries notaremos que los primeros 264 bytes de cada entrada corresponden a los atributos del dentry:

```c
    int numberOfInodes;
    int size;
    char name[256];
```

y luego se tendrán `numberOfInodes * sizeof(struct inode)` bytes correspondientes a los inodos que vivan en este directorio (junto con la data copiada al final de la metadata de cada inode).

La lectura y escritura del archivo se realizará al montar, desmontar y hacer un flush del filesystem.

Podemos ver el archivo que serializa un filesystem donde tenemos 2 archivos en root y 2 directorios, con 2 archivos para cada uno.

Los archivos se estructuran de la siguiente forma:

```
    \
    |- file1
    |- file2
    \- dir1
        |- file1
        |- file2
    \- dir2
        |- file1
        |- file2
```

Y el archivo serializador generado guarda así la información.

```
                       First Byte
                      +----------------------------------------------+
                    4 |             Number of Root Inodes            | 
                      +----------------------------------------------+
                    4 |          Number of Root Directories          | 
                      +----------------------------------------------+
4 + 4 + 256 + 1 = 265 | [ROOT INODE 1] Inumber, Size 1, Name & Type  | 
                      +----------------------------------------------+
4 + 4 + 256 + 1 = 265 | [ROOT INODE 2] Inumber, Size 2, Name & Type  |
                      +----------------------------------------------+ 
        4 + 256 = 260 |        [DIR 1] Number of Inodes, Name        | 
                      +----------------------------------------------+
4 + 4 + 256 + 1 = 265 | [DIR 1 INODE 1] Inumber, Size 3, Name & Type | 
                      +----------------------------------------------+
4 + 4 + 256 + 1 = 265 | [DIR 1 INODE 2] Inumber, Size 4, Name & Type | 
                      +----------------------------------------------+
        4 + 256 = 260 |        [DIR 2] Number of Inodes, Name        | 
                      +----------------------------------------------+
4 + 4 + 256 + 1 = 265 | [DIR 2 INODE 1] Inumber, Size 5, Name & Type | 
                      +----------------------------------------------+
4 + 4 + 256 + 1 = 265 | [DIR 2 INODE 2] Inumber, Size 6, Name & Type | 
                      +----------------------------------------------+
               Size 1 |             [ROOT FILE 1 DATA]               | 
                      +----------------------------------------------+
               Size 2 |             [ROOT FILE 2 DATA]               | 
                      +----------------------------------------------+
               Size 3 |            [DIR 1 INODE 1 DATA]              | 
                      +----------------------------------------------+
               Size 4 |            [DIR 1 INODE 2 DATA]              | 
                      +----------------------------------------------+
               Size 5 |            [DIR 2 INODE 1 DATA]              | 
                      +----------------------------------------------+
               Size 6 |            [DIR 2 INODE 2 DATA]              | 
                      +----------------------------------------------+
                                                            Last Byte
```

En el diagrama el tope indica el inicio del archivo y el piso el final. Vemos que primero tenemos la información general de nuestro filesystem. Luego tenemos la metadata e información de cada archivo en root. Finalmente tenemos la metadata de cada directorio junto con los inodos correspondientes a cada uno.

## Pruebas

Mostraremos algunas pruebas que se realizaron para verificar el correcto funcionamiento del filesystem.

Se provee el archivo `gendisk.c` que al ser compilado (junto con `filesystem.c`) genera un archivo con un disco de ejemplo. Sobre el mismo ejecutamos las pruebas del trabajo práctico. El mismo tiene la siguiente estructura:

```
-> Filesystem:
   Number of inodes: 3
   Number of dentries: 2
-> Inodes:
	File name: file 0
	File size: 4
	File type: c
	Times Open: 0
	Data: aaaa

	File name: file 1
	File size: 5
	File type: c
	Times Open: 0
	Data: bbbbb

	File name: file 2
	File size: 6
	File type: c
	Times Open: 0
	Data: cccccc
-> Dentries:
	Dir name: dir 0 with 2 inodes
		File name: file 0
		File size: 6
		File type: c
		Times Open: 0
		Data: aaaaaa

		File name: file 1
		File size: 7
		File type: c
		Times Open: 0
		Data: bbbbbbb

	Dir name: dir 1 with 2 inodes
		File name: file 2
		File size: 6
		File type: c
		Times Open: 0
		Data: aaaaaa

		File name: file 3
		File size: 7
		File type: c
		Times Open: 0
		Data: bbbbbbb

```

Aclaramos también que el archivo `rerun.sh` es un script que compila el fs y lo ejecuta.

### touch

```shell
$ ls prueba
'dir 0'  'dir 1'  'file 0'  'file 1'  'file 2'
$ touch "prueba/file 3"
$ ls prueba
'dir 0'  'dir 1'  'file 0'  'file 1'  'file 2'	'file 3'
$ touch "prueba/dir 0/file 3"
$ ls "prueba/dir 0"
'file 0'  'file 1'  'file 3'
$ touch "prueba/dir 1/file 4"
$ ls "prueba/dir 1"          
'file 2'  'file 3'  'file 4'
```

### mkdir

```shell
$ mkdir "prueba/dir 2"
$ ls prueba
'dir 0'  'dir 1'  'dir 2'  'file 0'  'file 1'  'file 2'
$ ls "prueba/dir 2"
$ touch "prueba/dir 2/file 0"
$ ls prueba
'dir 0'  'dir 1'  'dir 2'  'file 0'  'file 1'  'file 2'
$ ls "prueba/dir 2"
'file 0'
```

### ls

```shell
$ mkdir "prueba/dir 2"
$ ls prueba
'dir 0'  'dir 1'  'file 0'  'file 1'  'file 2'
```

### cat

```shell
$ cat "prueba/file 1"
bbbbb
```

### more

Si ejecutamos el siguiente script:

```shell
for i in $(seq 100)
do
    echo "$i" >> "prueba/file 0"
done
```

Veremos como el archivo se llena de números del 1 al 100. Esto facilita ver el funcionamiento de `more "prueba/file 0"` (el cual no podemos copiar y pegar por el formateado del texto).

### less

Ocurre lo mismo que en el caso anterior.

### stat


### rm

```shell

$ ls prueba
'dir 0'  'dir 1'  'file 0'  'file 1'  'file 2'
$ ls "prueba/dir 0"
'file 0'  'file 1'
$ rm "prueba/file 0"
$ ls prueba
'dir 0'  'dir 1'  'file 1'  'file 2'
$ rm "prueba/dir 0/file 0"
$ ls "prueba/dir 0"       
'file 1'
```

### rmdir

```shell
$ rmdir "prueba/dir 0"
$ ls prueba
'dir 1'  'file 0'  'file 1'  'file 2'
```


### write

```shell
$ cat "prueba/file 1"
bbbbb
$ echo "hi" > "prueba/file 1"   
$ cat "prueba/file 1"        
hi
```

```shell
$ cat "prueba/file 0"              
aaaa
$ echo "BBBBBBB" >> "prueba/file 0"
$ cat "prueba/file 0"              
aaaaBBBBBBB
$ cat "prueba/dir 0/file 0"        
aaaaaa
$ echo "BBBBBBB" >> "prueba/dir 0/file 0"
$ cat "prueba/dir 0/file 0"              
aaaaaaBBBBBBB
```

```shell
$ echo "text" > "prueba/newfile"  
$ ls prueba
'dir 0'  'dir 1'  'file 0'  'file 1'  'file 2'	 newfile
$ cat prueba/newfile
text
```