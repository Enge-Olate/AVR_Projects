#!/bin/bash

if [ ! -f "Doxyfile" ]; then
    echo "Erro: Doxyfile não encontrado!"
    exit 1
fi

echo "Gerando documentação..."

doxygen Doxyfile

echo "Documentação gerada."