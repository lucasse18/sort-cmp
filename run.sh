#!/bin/bash
trap exit ERR


# parâmetros
# ----------
K_MAX="16"
K_MIN="3"
SAMPLES="25"

# funcoes
# -------
die() {
  echo -e "$@"; exit 1
}

run() {
  # argumentos
  # $1 = codigo do algoritmo [1-7]

  # inicializa arquivos de saida, apagando runs anteriores para o mesmo algoritmo
  for order in asc desc rand part; do
    echo "# input_size,sorting_time(ns)" > ./data/out_${1}_${order}.csv
  done

  # executa o programa para cada k
  echo "executando algoritmo ${1}..."
  for ((i = ${K_MIN}; i <= ${K_MAX}; i++)); do
    #sudo nice -n -10 ./bin/sort -a ${1} -k ${i} -s ${SAMPLES} "${INPUT}_${i}"
    ./bin/sort -a ${1} -k ${i} -s ${SAMPLES} "${INPUT}_${i}"

    # informa progresso
    echo -ne "\riteração $((${i}-${K_MIN}+1))/$((${K_MAX}-${K_MIN}+1)) para k = ${i}"
  done
  echo "" # new line
}

# início do script
# ----------------
type make > /dev/null 2>&1 || die "GNU make nao foi encontrado no PATH."

make bin/sort

[ -s data/input_int_3 ]    || make bin/gen_input && bin/gen_input ${K_MAX}

for ((l = 1; l <= 6; l++)); do
  INPUT="./data/input_int"
  run ${l}
done

# bucket sort deve ser executado com a entrada do tipo ponto flutuante
INPUT="./data/input_float"
run 7

echo -e "resultados escritos no diretório 'data'."

