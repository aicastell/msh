---------------------------------------------------------------------------
El shell msh.
---------------------------------------------------------------------------

Este fichero contiene informacion que debe servir como ayuda a la hora de
comprender el funcionamiento del shell, asi como de su instalacion.


1. Instalacion.

Para compilar el shell teclee en la linea de comandos:

    $ make

El  programa  se compilara,  creando  por defecto un ejecutable llamado msh.
Para eliminar todos los ficheros objetos (.o) generados mediante una macro
incluida en el makefile, teclea:

    $ make clean




2. Tratamiento de la linea de entrada.

La linea de entrada se trata en la funcion gexe_rellenar, definida en el
modulo gexe.h.c. En este modulo se define la primera variable global del
shell. Pese a que no es recomendable su utilizacion, en este caso me ha
simplificado mucho el trabajo a la hora de pasar la informacion que contiene a
las diferentes funciones del programa. Esta variable global es EXE, cuya
estructura es la siguiente. Para EXE[i]:

                    +---------------------------------------+
    VCommand	    |  |  |  |  |  |  |  |  |   |  |  |  |  |
                    +---------------------------------------+
                    |               BackGround              |
                    +---------------------------------------+
    Redirecciones	|   <	|   >	|   >>	|   2>	|   2>>	|
                    +---------------------------------------+

La estructura EXE[i] almacena toda la informacion relativa al comando i-esimo.
Asi por ejemplo, para una linea de entrada como la siguiente:

    LineCommand="ls -l -ai >salida 2>errores &

se genera la siguiente estructura:

    EXE[0].VCommand[0]="ls"
    EXE[0].VCommand[1]="-l"
    EXE[0].VCommand[2]="-ai"
    EXE[0].VCommand[3]=NULL
    ...
    EXE[0].VCommand[MAXPARAM]=NULL
    EXE[0].BackGround=1
    EXE[0].Redirecciones[0]=""
    EXE[0].Redirecciones[1]="salida"
    EXE[0].Redirecciones[2]=""
    EXE[0].Redirecciones[3]="errores"
    EXE[0].Redirecciones[4]=""

Lo mismo  ocurre cuando tenemos varios comandos encadenados por tuberias. Cada
estructura EXE[i] contiene la informacion relativa a cada comando en cuestion.

El campo BackGround puede parecer redundante puesto que tansolo es necesario
uno a priori. Sin embargo, manteniento un campo BackGround por cada comando
ejecutable es posible saber cuando un backbround esta situado en un lugar
incorrecto, por ejemplo en la siguiente situacion:

    $ ls -l -a & | grep "casa" | more

El fragmento de codigo donde se trata la cadena de entrada es un poco largo,
pero se ha conseguido controlar cadenas de entrada erroneas tales como las
siguientes:

    comando >>...> salida   No existe redireccion
    comando <...< salida    No existe redireccion
    comando <               Falta parametro
    comando 2>              Falta parametro
    comando 2>>             Falta parametro
    comando >>              Falta parametro
    comando >               Falta parametro




3. Tratamiento de variables de entorno.

Todo este tratamiento se ha llevado a cabo en el modulo entorno.h.c. Para
llevar a cabo dicho tratamiento, se genera internamente una tabla de variables
de entorno global llamada ENTORNO, cuya estructura es la siguiente:

    +--------------------------------+
    |  nombre  |  valor   | exported |
    +----------+---------------------+
    |          |          |          | <- ENTORNO.next_empty
    +----------+---------------------+
    |          |          |          |
    +----------+---------------------+

La  idea  es  muy  sencilla.  La  tabla  ENTORNO  se  inicializa  con todos los
valores  de la variable  environ que se han heredado del padre del shell.  Cada
vez  que  se  inserta  una  variable  exportada,  o  se  actualiza una variable
exportada, se  llama  a entorno_to_environ,  de forma que todo proceso hijo del
shell reciba una copia totalmente actualizada del entorno del padre. Se aceptan
los siguientes comandos:

    set                         Lista todas variables entorno
    env                         Lista variable environ
    export                      Lista variables exportadas
    export a=b                  Exporta a=b
    export a=b c=d e=d ...      Exporta a, c, e, ...
    a=10                        Crea variable de entorno a=10
    PS1=hola                    Asigna nuevo valor a PS1
    VERSION=vertical            Idem antes.

La  variable VERSION  es  propia  de  mi  shell,   no  la  hereda del padre. Se
puede  utilizar para cambiar la version de shell activa: vertical o horizontal.
Cualquier otro valor de esta variable impide el funcionamiento correcto del shell.

Se ha logrado en parte que funcione el comando echo. Para ello se llama a la
funcion entorno_to_EXE que en si cambia el valor de cada variable de entorno de
la estructura EXE por su valor real. En este sentido funcionan comandos como:

    $ cd $HOME
    $ cp archivo $HOME/uno/dos
    ...

LIMITACION. Sin embargo, puesto que es un tratamiento, mas o menos "a pelo", la
funcion isalnum limita el funcionamiento correcto a aquellas variables de
entorno compuestas solamente de caracteres y o numeros, pero no sirve para
variables tipo LS_COLORS, puesto que el caracter '_' estropea el invento.



4. Tratamiento de alias.

Con la misma idea con la que se han gestionado las variables de entorno (con
sus particularidades), se implementan los alias con una tabla ALIAS global tal
como la siguiente:

    |---------------|
    | nombre| apodo |
    |---------------|
    |       |       | <- ALIAS.next_empty
    |---------------|
    |       |       |
    |---------------|

Todos los comandos que se listan a continuación (con sus variantes) funcionan:

    $ alias                     Lista toda la tabla ALIAS
    $ alias a                   Lista alias a
    $ alias a=b                 Introduce alias a=b
    $ alias la='ls -l'          Introduce la='ls -l'
    $ alias ll="la -l"          Introduce ll="la -l"
    $ unalias a                 Borra a de ALIAS
    $ unalias a b c             Borra los alias a b y c
    $ alias a a=b a='ls -l'     Hace lo esperado

Funcionan tanto los delimitadores '' como "". En el caso del alias ll="la -l" ,
la ejecucion  de  ll  es  la esperada.

LIMITACIÓN. Cadenas como la siguiente, donde se usan tuberías, NO funcionan.

    $ alias lm='ls -l | more'

El problema es debido a que al tratar la cadena de entrada con strtok, se
divide la entrada por caracteres '|', con lo que al no estar todo en el mismo
VCommand, había que tratar este caso como un caso especial, y por no complicar
mas la implementación, se ha decidido no gestionar alias de este tipo.


5. La espera de los hijos.

Despues de haber peleado largamente con la señal SIGCHLD, me parecio tarea
imposible hacer la espera de los hijos con esta señal, puesto que cada vez que
se moria un hijo se me activaba la señal y no tenia forma de distinguir si el
hijo muerto era el que estaba en BackGround, o no.

Finalmente se decidió utilizar un simple vector global PID en el que cada vez
que se lanza un proceso en background se guarda su pid, y cuando se hace exit
se hace un waitpid de todos los  pids de ese vector. Asi de sencillo. Esta
estructura, asi como las funciones que la gestionan se encuentra definida en el
modulo pid.h.c

Para probar su funcionamiento lanzar varios procesos en background y a
continuación finalizar el shell con el comando "exit". Es decir:

    $ sleep 20&
    $ sleep 10&
    $ exit

