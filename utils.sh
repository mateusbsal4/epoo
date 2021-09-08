#!/usr/bin/env bash

function utilizacao {
    echo 'Utilizacao'
    echo ""$0" [-h -r -t] [-vt] [-c | -C source_dir build_dir] [-b build_dir]"
    echo '-h | --help'
    echo '  Imprime esta mensagem'
    echo '-r | --run'
    echo '  Executa o programa compilado em C++'
    echo '-t | --test'
    echo '  Executa os testes fornecidos no diretorio test/'
    echo '-vt | --verbose-test'
    echo '  Executa os testes e mostra saida dos casos de falha'
    echo '-c  source_dir build_dir'
    echo '  Reanaliza os arquivos CMakeLists.txt'
    echo '-C source_dir build_dir'
    echo '  Reanaliza os arquivos CMakeLists.txt e habilita testes'
    echo '-b | --build build_dir'
    echo '  Compila os arquivos C++ para serem executados'
    echo '--build-and-run build_dir'
    echo '  Compila os arquivos C++ e executa o programa gerado'
}

function confere_se_maior_igual {
    if [ $1 -ge $2 ]
    then
        exit 0
    fi
    echo 'Numero de argumentos insuficiente'
    echo ''
    utilizacao
    exit 1
}

CMAKE_BIN_PATH=cmake/bin
EXECUTABLE_NAME=EP_LabOO

if [ $# -eq 0 ]
then
    utilizacao
fi


opcao="$1"
case $opcao in
    -r|--run )
        ./build/src/"$EXECUTABLE_NAME"
        ;;
    -t|--test )
        "$CMAKE_BIN_PATH"/ctest \
            --test-dir build/test 
        ;;
    -vt|--verbose-test )
        "$CMAKE_BIN_PATH"/ctest \
            --test-dir build/test \
            --output-on-failure
        ;;
    -c )
        (confere_se_maior_igual $# 3) || exit $? 
        "$CMAKE_BIN_PATH"/cmake \
            -S "$2" \
            -B "$3"
        ;;
    -C )
        (confere_se_maior_igual $# 3) || exit $? 
        "$CMAKE_BIN_PATH"/cmake \
            -S "$2" \
            -B "$3" \
            -DENABLE_TESTING=ON
        ;;
    -b|--build )
        (confere_se_maior_igual $# 2) || exit $? 
        "$CMAKE_BIN_PATH"/cmake \
            --build "$2"
        ;;
    --build-and-run )
        (confere_se_maior_igual $# 2) || exit $? 
        "$CMAKE_BIN_PATH"/cmake \
            --build "$2"
        ./build/src/"$EXECUTABLE_NAME"
        ;;
    * )
        utilizacao
        exit 1
        ;;
esac

exit 0
