#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <chrono>

struct TokenData {
    int termFrequency;
    double weight;
};

void calculateTfIdfWeights(std::unordered_map<std::string, TokenData>& tokens, int totalDocuments) {
    for (auto& tokenPair : tokens) {
        int documentFrequency = tokenPair.second.termFrequency;
        double tf = static_cast<double>(documentFrequency);
        double idf = std::log(static_cast<double>(totalDocuments) / (1 + documentFrequency));
        tokenPair.second.weight = tf * idf;
    }
}

void updatePostingFile(const std::string& filePath, const std::unordered_map<std::string, TokenData>& tokens) {
    std::ofstream postingFile(filePath);
    for (const auto& tokenPair : tokens) {
        postingFile << tokenPair.first << " " << tokenPair.second.weight << "\n";
    }
}

int main() {
    std::unordered_map<std::string, TokenData> tokens;
    int totalDocuments = 100;

    std::ifstream inputFile("tokens.txt");
    if (!inputFile) {
        std::cerr << "Error al abrir el archivo de entrada tokens.txt\n";
        return 1;
    }

    std::string token;
    int frequency;

    while (inputFile >> token >> frequency) {
        tokens[token].termFrequency = frequency;
    }
    inputFile.close();

    auto start = std::chrono::high_resolution_clock::now();

    calculateTfIdfWeights(tokens, totalDocuments);
    updatePostingFile("posting.txt", tokens);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::ofstream logFile("a10_matricula.txt");
    logFile << "Tiempo de procesamiento: " << duration.count() << " segundos\n";
    logFile.close();

    return 0;
}
