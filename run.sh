#!/bin/bash
trap exit ERR


# argumentos
# $1 = seed (opcional)
#      usada somente quando os dados de entrada não estão presentes
SEED=${1}

# parametros
K_MAX="16"
K_MIN="10"

# funcoes
die() {
  echo -e "$@"; exit 1
}

run() {
  # argumentos
  # $1 = codigo do algoritmo [1-7]

  # inicializa arquivos de saída
  for data_order in asc desc rand part; do
    echo "#k,sorting_time" > "./data/out_${1}_${data_order}.csv"
  done

  # executa o programa para cada k
  echo "executando algoritmo ${1}..."
  for ((i = ${K_MIN}; i <= ${K_MAX}; i++)); do
    # xargs troca '\n' por ' '
    OUT=$(./bin/sort -a ${1} -k ${i} ./data/input_int | xargs)

    # separa a saída do programa para cada ordem de dados
    j=1;
    for data_order in asc desc rand part; do
      echo ${OUT} | cut -d' ' -f${j} >> "./data/out_${1}_${data_order}.csv"
      ((j++))
    done

    # informa progresso
    echo -ne "\riteração $((${i}-${K_MIN}+1))/$((${K_MAX}-${K_MIN}+1)) para k = ${i}"
  done
  echo "" # new line
}

# início do script
# ----------------
type make > /dev/null 2>&1 || die "GNU make nao foi encontrado no PATH."

[ -x ./bin/sort ]         || make bin/sort
[ -x ./bin/gen_input ]    || make bin/gen_input
[ -s ./data/input_int ]   || bin/gen_input 18 ${SEED}
[ -s ./data/input_float ] || bin/gen_input 18 ${SEED}

for ((l = 1; l <= 4; l++ )); do
  run ${l}
done

echo -e "resultados escritos no diretório 'data'."

