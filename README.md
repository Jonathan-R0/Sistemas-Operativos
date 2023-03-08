# Sistemas-Operativos

Trabajos prácticos y desafios de Sistemas Operativos 2022 1C, FIUBA 📟

# Introducción 

- ¿Qué es un sistema operativo?
- ¿Qué es el time sharing?
- ¿Qué es la virtualización? (en términos generales)
- Tres atributos claves de los sistemas operativos:
    - ¿Qué significa que el sistema operativo sea referee?
    - ¿Qué significa que el sistema operativo sea ilusionista?
    - ¿Qué significa que el sistema operativo sea sticky?
- ¿Qué es el kernel?
- Enumerar los modos básicos de ejecución. 
- ¿Qué es un proceso?
- ¿Qué tipos de programa puede ejecutar la shell?

<div align="right">Páginas 0 a 3</div>

# Kernel

- ¿Cómo inicializa el sistema operativo un proceso?
    - ¿Qué problemas trae?
    - ¿Cómo se solucionan?
- ¿Qué es el dual mode?
    - ¿Cómo se implementa esto último?
    - ¿Qué modos de ejecución existen?
    - ¿Dónde se ve esto último?
- ¿Cómo se implementa la protección al sistema?
- ¿Cómo se implementa la protección de memoria?
- ¿Qué es y cómo se implementa un timer interrumpt?
- ¿Qué formas hay de ir de user a kernel mode?
    - ¿Qué es una interrupción y como funciona?
    - ¿Qué es una excepción y como funciona?
    - ¿Qué es una syscall y como se utiliza?
- ¿Qué formas hay de ir de kernel a user mode?
    - ¿Cómo se crea un nuevo proceso?
- ¿Qué tipos de kernel existen?
- ¿Cómo se inicia el sistema operativo y el kernel?
- ¿De qué partes consta el booteo?
- ¿De qué partes consta la carga del kernel?
- ¿De qué partes consta el inicio de aplicaciones de usuario?
- ¿Qué se hace al inicio del kernel?
    - ¿Qué hace `start_kernel`?
- ¿Qué hace el primer proceso?

<div align="right">Páginas 4 a 10</div>

# Procesos

- ¿Qué es un programa?
- ¿De qué se encarga el sistema operativo?
- ¿Qué incluye un proceso?
    - ¿Qué continene cada thread?
- ¿Qué es la virtualización de la memoria?
- ¿En cuántas partes se divide la memoria?
- ¿Cómo se protege la memoria?
- ¿Cómo se traduce la memoria?
    - ¿Qué es el MMU?
- ¿Qué es la virtualización del procesador?
- ¿Qué es el contexto de un proceso?
- ¿Cómo puede un usuario crear un proceso?
    - ¿Qué es un fork?
    - ¿Cómo funciona el algorítmo de `fork(2)`?
- ¿Qué estados puede tener un proceso en Linux?

<div align="right">Páginas 10 a 14</div>

# Scheduling

- ¿Qué es el scheduling?
- ¿Cómo divide la máquina el tiempo?
- ¿Qué es el time sharing?
- ¿Qué es la multiprogramación?
- ¿Cuál es la probabilidad de $n$ procesos con gasto de tiempo $p$ estén haciendo i/o?
- ¿Y la probabilidad de que estén utilizando el CPU?
- ¿Qué métodos de planificación de procesos existen?
    - ¿Qué es el FIFO?
    - ¿Qué es el SJF?
    - ¿Qué es el STC?
    - ¿Qué es el tiempo de respuesta?
    - ¿Qué es el RR?
        - ¿Qué desventajas trae?
    - ¿Qué es el MLFQ?
        - ¿Qué reglas báscias tiene?
        - ¿Qué problemas trae? 
        - ¿Cómo se solucionan?
    - ¿Qué es el proportional share?
    - ¿Qué es el mecanismo de lotería?
        - ¿Cómo funciona?
        - ¿Cómo se implementa?
    - ¿Qué es la ley de Amdahl?
    - ¿Qué es la arquitectura multiprocesador?
    - ¿Qué es la caché?
        - ¿Qué es la temporalidad temporal?
        - ¿Qué es la temporalidad espacial?
    - ¿Qué es la coherencia de caché?
    - ¿Qué es la afinidad de caché?
    - ¿Qué es la SQMS?
        - ¿Qué desventajas trae?
    - ¿Qué es la planificación multi-queue?
        - ¿Qué desventajas trae?
    - ¿Qué ocurre en la vida real?
        - ¿Cómo se implementa el scheduling en Linux?
        - ¿Cómo se manejan los context switches?
        - ¿Cómo se ponderan los procesos?
        - ¿Cómo se encuentra el próximo job a ejecutar?

<div align="right">Páginas 15 a 27</div>

# Memoria Virtual

- ¿Cómo crecen el heap y el stack?
- ¿Qué hace `brk`?
- ¿Qué es la traducción de direcciones?
    - ¿Qué características tiene?
- ¿Qué es la traducción base and bound?
- ¿Qué es la traducción con tabla de segmentos?
    - ¿Cómo se compone la dirección?
- ¿Qué es una página?
    - ¿Cómo se diferencia de la memoria segmentada?
    - ¿Qué beneficios trae?
    - ¿Qué desventajas trae?
    - ¿Cómo se compone la dirección?
    - ¿Cómo se traduce?
- ¿Qué es el multi-level address translation?
    - ¿Qué beneficios trae?
    - ¿Cómo se compone la dirección?
    - ¿Cómo se traduce?
- ¿Cómo se optimiza la búqueda de las páginas?
    - ¿Qué es la TLB?
    - ¿Qué es la caché de páginas?
    - ¿Qué ocurre en un TLB MISS?
    - ¿Cuándo y cómo se borra la TLB?

<div align="right">Páginas 27 a 44</div>

# Concurrencia

- ¿Qué es la concurrencia?
- ¿Qué es un thread?
- ¿Cómo manipula al thread el sistema operativo?
- ¿Qué contiene un thread?
- ¿Cómo se compara un thread con un proceso?
- ¿Cómo se relacionan dos threads?
- ¿Cómo es la api de threads en C?
- ¿Qué es el TCB?
    - ¿Qué información es individual a cada thread?
    - ¿Qué información es compartida entre threads?
- ¿Qué estados tiene un thread?
- ¿Qué diferencias hay entre un thread y un proceso?
- ¿Qué es una race condition?
- ¿Qué es una operaición atómica?
- ¿Qué es un lock?
    - ¿Qué propiedades tiene?
    - ¿Cómo es la api de locks en C?
    - ¿Qué debe asegurar un lock?
    - ¿Qué es una sección crítica?
    - ¿Qué es una condition variable?
    - ¿Qué es un monitor?
    - ¿Qué tipos de locks hay?
- ¿Qué es un deadlock?
    - ¿Qué bugs que no sean de tipo deadlock existen?
    - ¿Qué cuidados se deben tener para evitar un deadlock?
- ¿Qué es detect and recover?

<div align="right">Páginas 45 a 56</div>

(no sé de quien es el resumen pero me sirvió para repasar lmao)
