#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <algorithm>

const int BOARD_SIZE = 8;
const std::string PIECES = "KQRBNPkqrbnp";

enum Player { HUMAN, AI };

struct Move {
    int startX, startY, endX, endY;
};

class Board {
public:
    char board[8][8];

    Board() {
        const std::string initialBoard =
            "rnbqkbnr"
            "pppppppp"
            "........"
            "........"
            "........"
            "........"
            "PPPPPPPP"
            "RNBQKBNR";

        for (int i = 0; i < 64; ++i)
            board[i / 8][i % 8] = initialBoard[i];
    }

    void print() const {
        std::cout << "  a b c d e f g h\n";
        for (int i = 0; i < BOARD_SIZE; ++i) {
            std::cout << 8 - i << " ";
            for (int j = 0; j < BOARD_SIZE; ++j) {
                std::cout << board[i][j] << ' ';
            }
            std::cout << 8 - i << "\n";
        }
        std::cout << "  a b c d e f g h\n";
    }

    bool isValidMove(const Move& move) const {
        // Simplified move validation
        return move.startX >= 0 && move.startX < 8 && move.startY >= 0 && move.startY < 8 &&
            move.endX >= 0 && move.endX < 8 && move.endY >= 0 && move.endY < 8 &&
            board[move.startY][move.startX] != '.'; // Ensure the start position has a piece
    }

    void applyMove(const Move& move) {
        board[move.endY][move.endX] = board[move.startY][move.startX];
        board[move.startY][move.startX] = '.';
    }

    void undoMove(const Move& move, char capturedPiece) {
        board[move.startY][move.startX] = board[move.endY][move.endX];
        board[move.endY][move.endX] = capturedPiece;
    }

    char pieceAt(int x, int y) const {
        return board[y][x];
    }

    std::vector<Move> generateMoves() const {
        std::vector<Move> moves;
        for (int y = 0; y < 8; ++y) {
            for (int x = 0; x < 8; ++x) {
                if (board[y][x] != '.') {
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            int ny = y + dy;
                            int nx = x + dx;
                            if (ny >= 0 && ny < 8 && nx >= 0 && nx < 8) {
                                moves.push_back({ x, y, nx, ny });
                            }
                        }
                    }
                }
            }
        }
        return moves;
    }
};

class ChessAI {
public:
    Move findBestMove(Board& board, int depth) {
        Move bestMove;
        int bestValue = std::numeric_limits<int>::min();
        for (const auto& move : board.generateMoves()) {
            char capturedPiece = board.pieceAt(move.endX, move.endY);
            board.applyMove(move);
            int moveValue = minimax(board, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
            board.undoMove(move, capturedPiece);
            if (moveValue > bestValue) {
                bestMove = move;
                bestValue = moveValue;
            }
        }
        return bestMove;
    }

private:
    int evaluate(const Board& board) const {
        // Simplified evaluation
        return 0;
    }

    int minimax(Board& board, int depth, int alpha, int beta, bool maximizingPlayer) {
        if (depth == 0) {
            return evaluate(board);
        }

        if (maximizingPlayer) {
            int maxEval = std::numeric_limits<int>::min();
            for (const auto& move : board.generateMoves()) {
                char capturedPiece = board.pieceAt(move.endX, move.endY);
                board.applyMove(move);
                int eval = minimax(board, depth - 1, alpha, beta, false);
                board.undoMove(move, capturedPiece);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return maxEval;
        }
        else {
            int minEval = std::numeric_limits<int>::max();
            for (const auto& move : board.generateMoves()) {
                char capturedPiece = board.pieceAt(move.endX, move.endY);
                board.applyMove(move);
                int eval = minimax(board, depth - 1, alpha, beta, true);
                board.undoMove(move, capturedPiece);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break;
                }
            }
            return minEval;
        }
    }
};

Move parseMove(const std::string& moveStr) {
    return {
        moveStr[0] - 'a',
        8 - (moveStr[1] - '0'),
        moveStr[2] - 'a',
        8 - (moveStr[3] - '0')
    };
}

int main() {
    Board board;
    ChessAI ai;
    Player currentPlayer = HUMAN;

    while (true) {
        board.print();

        if (currentPlayer == HUMAN) {
            std::string moveStr;
            std::cout << "Enter your move (e.g., e2e4): ";
            std::cin >> moveStr;
            Move move = parseMove(moveStr);
            if (board.isValidMove(move)) {
                board.applyMove(move);
                currentPlayer = AI;
            }
            else {
                std::cout << "Invalid move!\n";
            }
        }
        else {
            Move bestMove = ai.findBestMove(board, 3);
            board.applyMove(bestMove);
            std::cout << "AI move: " << static_cast<char>(bestMove.startX + 'a') << 8 - bestMove.startY
                << static_cast<char>(bestMove.endX + 'a') << 8 - bestMove.endY << "\n";
            currentPlayer = HUMAN;
        }
    }

    return 0;
}
