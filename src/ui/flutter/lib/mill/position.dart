/*
  FlutterMill, a mill game playing frontend derived from ChessRoad
  Copyright (C) 2019 He Zhaoyun (ChessRoad author)
  Copyright (C) 2019-2020 Calcitem <calcitem@outlook.com>

  FlutterMill is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FlutterMill is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import '../mill/mill.dart';
import '../mill/recorder.dart';

class StateInfo {
  /*
  // Copied when making a move
  int rule50 = 0;
  int pliesFromNull = 0;


  get rule50 => _rule50;
  set rule50(int value) => _rule50 = value;

  get pliesFromNull => _pliesFromNull;
  set pliesFromNull(int value) => _pliesFromNull = value;
  */
}

class Position {
  GameResult result = GameResult.pending;
  String _sideToMove = Color.black;
  List<String> _board = List<String>(49); // 7  *  7
  MillRecorder _recorder;

  int rule50 = 0;
  int pliesFromNull = 0;

  Phase phase = Phase.none;
  Action action = Action.none;
  int pieceCountInHandBlack = 12;
  int pieceCountInHandWhite = 12;
  int pieceCountOnBoardBlack = 0;
  int pieceCountOnBoardWhite = 0;
  int pieceCountNeedRemove = 0;

  int gamePly = 0;

  StateInfo st;

  String them;
  String winner;
  GameOverReason gameOverReason = GameOverReason.noReason;

  Position.init() {
    for (var i = 0; i < _board.length; i++) {
      _board[i] ??= Piece.noPiece;
    }

    // Example
    //_board[sqToLoc[8]] = Piece.blackStone;

    _recorder = MillRecorder(lastCapturedPosition: fen());
  }

  Position.clone(Position other) {
    _board = List<String>();
    other._board.forEach((piece) => _board.add(piece));
    _sideToMove = other._sideToMove;
    _recorder = other._recorder;
  }

  /// fen() returns a FEN representation of the position.

  String fen() {
    var ss = '';

    // Piece placement data
    for (var file = 1; file <= 3; file++) {
      for (var rank = 1; rank <= 8; rank++) {
        //
        final piece = pieceOn(squareToIndex[makeSquare(file, rank)]);
        ss += piece;
      }

      if (file == 3)
        ss += ' ';
      else
        ss += '/';
    }

    // Active color
    ss += _sideToMove;

    ss += " ";

    // Phrase
    switch (phase) {
      case Phase.none:
        ss += "n";
        break;
      case Phase.ready:
        ss += "r";
        break;
      case Phase.placing:
        ss += "p";
        break;
      case Phase.moving:
        ss += "m";
        break;
      case Phase.gameOver:
        ss += "o";
        break;
      default:
        ss += "?";
        break;
    }

    ss += " ";

    // Action
    switch (action) {
      case Action.place:
        ss += "p";
        break;
      case Action.select:
        ss += "s";
        break;
      case Action.remove:
        ss += "r";
        break;
      default:
        ss += "?";
        break;
    }

    ss += " ";

    ss += pieceCountOnBoardBlack.toString() +
        " " +
        pieceCountInHandBlack.toString() +
        " " +
        pieceCountOnBoardWhite.toString() +
        " " +
        pieceCountInHandWhite.toString() +
        " " +
        pieceCountNeedRemove.toString() +
        " ";

    int sideIsBlack = _sideToMove == Color.black ? 1 : 0;

    ss +=
        rule50.toString() + " " + (1 + (gamePly - sideIsBlack) ~/ 2).toString();

    // step counter
    //ss += '${_recorder?.halfMove ?? 0} ${_recorder?.fullMove ?? 0}';

    print("fen = " + ss);

    return ss;
  }

  void putPiece(var pt, int index) {
    _board[index] = pt;
  }

  String move(int from, int to) {
    //
    if (!validateMove(from, to)) return null;

    final captured = _board[to];

    final move = Move(from, to, captured: captured);
    //StepName.translate(this, move);
    _recorder.stepIn(move, this);

    // 修改棋盘
    _board[to] = _board[from];
    _board[from] = Piece.noPiece;

    // 交换走棋方
    _sideToMove = Color.opponent(_sideToMove);

    return captured;
  }

  // 验证移动棋子的着法是否合法
  bool validateMove(int from, int to) {
    // 移动的棋子的选手，应该是当前方
    //if (Color.of(_board[from]) != _sideToMove) return false;
    return true;
    //(StepValidate.validate(this, Move(from, to)));
  }

  // 在判断行棋合法性等环节，要在克隆的棋盘上进行行棋假设，然后检查效果
  // 这种情况下不验证、不记录、不翻译
  void moveTest(Move move, {turnSide = false}) {
    //
    // 修改棋盘
    _board[move.to] = _board[move.from];
    _board[move.from] = Piece.noPiece;

    // 交换走棋方
    if (turnSide) _sideToMove = Color.opponent(_sideToMove);
  }

  bool regret() {
    //
    final lastMove = _recorder.removeLast();
    if (lastMove == null) return false;

    _board[lastMove.from] = _board[lastMove.to];
    _board[lastMove.to] = lastMove.captured;

    _sideToMove = Color.opponent(_sideToMove);

    final counterMarks = MillRecorder.fromCounterMarks(lastMove.counterMarks);
    _recorder.halfMove = counterMarks.halfMove;
    _recorder.fullMove = counterMarks.fullMove;

    if (lastMove.captured != Piece.noPiece) {
      //
      // 查找上一个吃子局面（或开局），NativeEngine 需要
      final tempPosition = Position.clone(this);

      final moves = _recorder.reverseMovesToPrevCapture();
      moves.forEach((move) {
        //
        tempPosition._board[move.from] = tempPosition._board[move.to];
        tempPosition._board[move.to] = move.captured;

        tempPosition._sideToMove = Color.opponent(tempPosition._sideToMove);
      });

      _recorder.lastCapturedPosition = tempPosition.fen();
    }

    result = GameResult.pending;

    return true;
  }

  String movesSinceLastCaptured() {
    //
    var steps = '', posAfterLastCaptured = 0;

    for (var i = _recorder.stepsCount - 1; i >= 0; i--) {
      if (_recorder.stepAt(i).captured != Piece.noPiece) break;
      posAfterLastCaptured = i;
    }

    for (var i = posAfterLastCaptured; i < _recorder.stepsCount; i++) {
      steps += ' ${_recorder.stepAt(i).step}';
    }

    return steps.length > 0 ? steps.substring(1) : '';
  }

  get manualText => _recorder.buildManualText();

  get side => _sideToMove;

  changeSideToMove() => _sideToMove = Color.opponent(_sideToMove);

  String pieceOn(int index) => _board[index];

  get halfMove => _recorder.halfMove;

  get fullMove => _recorder.fullMove;

  get lastMove => _recorder.last;

  get lastCapturedPosition => _recorder.lastCapturedPosition;
}
