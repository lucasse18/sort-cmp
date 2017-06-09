#!/bin/bash
set -e


# funcoes
die() {
  echo -e "$@"; exit 1
}

plot() {
  # argumentos
  # $1 = codigo do algoritmo [1-7]
  # $2 = ordem dos dados [asc,desc,rand,part]
  # $3 = titulo do grafico (string)
  # $4 = cor da linha em hexadecimal (#rrggbb)

#  case ${1} in
#    1 echo"" ;;
#  esac

  gnuplot << EndOfFile
  set datafile separator ","
  set title "${3}"
  set terminal svg enhanced size 500,500 background rgb 'white'

  set output "plots/plot_${1}_${2}.svg"
  set xlabel "Entrada 2^{x}"
  set ylabel "Tempo (ms)"
  set bmargin 3
  #set xtics 2
  #set ytics 0.02
  #set yrange [0:1]
  #set pointintervalbox 3
  set style line 1 lc rgb '${4}' lt 1 lw 2# pt 7 ps 0.5
  set grid

  plot "./data/out_${1}_${2}.csv" smooth cspline ls 1 notitle

EndOfFile
}

# início do script
# ----------------
type gnuplot > /dev/null 2>&1 || die "gnuplot nao foi encontrado no PATH."

plot 1 "asc"  "Insertion sort - Crescente"              "#000000"
plot 1 "desc" "Insertion sort - Decrescente"            "#ff0000"
plot 1 "rand" "Insertion sort - Aleatório"              "#00ff00"
plot 1 "part" "Insertion sort - Parcialmente Crescente" "#0000ff"

plot 2 "asc"  "Mergesort - Crescente"                   "#000000"
plot 2 "desc" "Mergesort - Decrescente"                 "#ff0000"
plot 2 "rand" "Mergesort - Aleatório"                   "#00ff00"
plot 2 "part" "Mergesort - Parcialmente Crescente"      "#0000ff"

plot 3 "asc"  "Heapsort - Crescente"                    "#000000"
plot 3 "desc" "Heapsort - Decrescente"                  "#ff0000"
plot 3 "rand" "Heapsort - Aleatório"                    "#00ff00"
plot 3 "part" "Heapsort - Parcialmente Crescente"       "#0000ff"

plot 4 "asc"  "Quicksort - Crescente"                   "#000000"
plot 4 "desc" "Quicksort - Decrescente"                 "#ff0000"
plot 4 "rand" "Quicksort - Aleatório"                   "#00ff00"
plot 4 "part" "Quicksort - Parcialmente Crescente"      "#0000ff"

echo "gráficos gerados no diretório 'plot'."

# fim do script
# -------------
