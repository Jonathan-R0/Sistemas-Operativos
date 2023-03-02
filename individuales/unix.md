# Tabla de Contenidos

- [PS](#ps)
    - [Ejemplo de Ejecución](#ejemplo-de-ejecución-de-ps)
- [Timeout](#timeout)
    - [Ejemplo de Ejecución](#ejemplo-de-ejecución-de-timeout)
# PS

Siendo que dentro de `proc` se encuentra la información de los procesos, abrimos el directorio en cuestión. Posteriormente empezamos a leer todos los directorios disponibles y nos quedamos con los que sean números. Estos números representan el PID del proceso a analizar. Para cada uno de estos accedemos a su correspondiente archivo de estadísticas, lo leemos e imprimimos la inforamción que nos interesa. 

## Ejemplo de Ejecución de PS

Adjuntamos un ejemplo del inicio del output de nuestro programa:

```
$ ./ps      
      DIR COMMAND
	1 systemd
	2 kthreadd
	3 rcu_gp
	4 rcu_par_gp
	5 netns
	7 kworker/0:0H-events_highpri
	9 kworker/0:1H-events_highpri
	10 mm_percpu_wq
	11 rcu_tasks_rude_
	12 rcu_tasks_trace
	13 ksoftirqd/0
	14 rcu_sched
	15 migration/0
	16 idle_inject/0
	17 kworker/0:1-cgroup_destroy
	18 cpuhp/0
	19 cpuhp/1
	20 idle_inject/1
	21 migration/1
	22 ksoftirqd/1
	24 kworker/1:0H-events_highpri
	25 cpuhp/2
	26 idle_inject/2
	27 migration/2
	28 ksoftirqd/2
	30 kworker/2:0H-kblockd
```

Y lo comparamos contra el comando recomendado por la cátedra para comparar los resultados:

```
$ ps -eo pid,comm
    PID COMMAND
      1 systemd
      2 kthreadd
      3 rcu_gp
      4 rcu_par_gp
      5 netns
      7 kworker/0:0H-events_highpri
      9 kworker/0:1H-events_highpri
     10 mm_percpu_wq
     11 rcu_tasks_rude_
     12 rcu_tasks_trace
     13 ksoftirqd/0
     14 rcu_sched
     15 migration/0
     16 idle_inject/0
     17 kworker/0:1-cgroup_destroy
     18 cpuhp/0
     19 cpuhp/1
     20 idle_inject/1
     21 migration/1
     22 ksoftirqd/1
     24 kworker/1:0H-events_highpri
     25 cpuhp/2
     26 idle_inject/2
     27 migration/2
     28 ksoftirqd/2
     30 kworker/2:0H-kblockd
```

# Timeout

Al principio verificamos la cantidad de elementos recibidos por argumento y que el segundo corresponda a un número. De lo contrario setteamos `errno` con el código de error `EINVAL` _(Error Invalid Argument)_.

Posteriormente mappeamos la señal SIGUSR1 al callback que mata al proceso hijo. Luego configuramos el evento y el timer en función del tiempo recibido por parámetro en segundos. Finalmente creamos el proceso hijo y esperamos a que termine. Si el proceso hijo no termina en el tiempo especificado, se envía la señal SIGUSR1 al proceso hijo y se espera a que termine. 

## Ejemplo de Ejecución de Timeout

El siguiente programa muestra el funcionamiento de nuestro timeout. Tomamos un punto de inicio $t_0 \approx 4.3$ y tiempo de finalización $t_1 \approx 6.3$. Dando un tiempo de ejecución de $t_1 - t_0 \approx 2$ segundos.

```bash
$ time; ./timeout 2 sleep 5; time
shell  0,01s user 0,01s system 0% cpu 4,338 total
children  0,00s user 0,00s system 0% cpu 4,338 total
shell  0,01s user 0,01s system 0% cpu 6,341 total
children  0,00s user 0,00s system 0% cpu 6,341 total
```

Comparado contra la ejecución del comando `timeout`:

```bash
$ time; timeout 2 sleep 5; time 
shell  0,01s user 0,01s system 0% cpu 3:59,52 total
children  0,00s user 0,00s system 0% cpu 3:59,52 total
shell  0,01s user 0,01s system 0% cpu 4:01,53 total
children  0,00s user 0,00s system 0% cpu 4:01,53 total
```

Donde nuestro programa tarda $\Delta t_{\text{fisop}} = 2.003$ y el comando `timeout`  $\Delta t_{\text{std}} = 2.010$ segundos. Resultó ser relativamente similar en tiempo de ejecución.
