// This file is part of Sanmill.
// Copyright (C) 2019-2023 The Sanmill developers (see AUTHORS file)
//
// Sanmill is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Sanmill is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QPainter>

#include "boarditem.h"
#include "graphicsconst.h"
#include "types.h"

BoardItem::BoardItem(const QGraphicsItem *parent)
    : boardSideLength(BOARD_SIDE_LENGTH)
{
    Q_UNUSED(parent)

    // Put center of the board in the center of the scene
    setPos(0, 0);

    initPoints();
}

BoardItem::~BoardItem() = default;

/**
 * @brief Get the bounding rectangle of the board item.
 *
 * This function returns a QRectF that represents the bounding box
 * around the board item. The bounding box is calculated based on
 * the dimensions of the board and includes extra space for the shadow.
 *
 * @return QRectF - Bounding rectangle with dimensions adjusted for shadow.
 */
QRectF BoardItem::boundingRect() const
{
    return QRectF(-boardSideLength / 2, -boardSideLength / 2,
                  boardSideLength + boardShadowSize,
                  boardSideLength + boardShadowSize);
}

/**
 * @brief Get the shape of the board item for interaction.
 *
 * This function returns a QPainterPath that represents the interactive
 * shape of the board item. The shape is used for interactive features.
 *
 * In the current implementation, the shape is the same as the bounding
 * rectangle.
 *
 * @return QPainterPath - Shape of the board item.
 */
QPainterPath BoardItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());

    return path;
}

void BoardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    drawBoard(painter);
    drawLines(painter);
    drawCoordinates(painter);
#ifdef PLAYER_DRAW_SEAT_NUMBER
    drawPolarCoordinates(painter);
#endif
}

void BoardItem::setDiagonal(bool enableDiagonal)
{
    hasDiagonalLine = enableDiagonal;
    update(boundingRect());
}

void BoardItem::initPoints()
{
    // Initialize 24 points
    for (int f = 0; f < FILE_NB; f++) {
        // The first point corresponds to the 12 o'clock position on the inner
        // ring, followed by points arranged in a clockwise direction. This
        // pattern is replicated for the middle and outer rings as well.
        const int radius = (f + 1) * LINE_INTERVAL;
        const int clockwiseRingCoordinates[][2] = {{0, -radius}, {radius, -radius},
                                           {radius, 0},  {radius, radius},
                                           {0, radius},  {-radius, radius},
                                           {-radius, 0}, {-radius, -radius}};
        for (int r = 0; r < RANK_NB; r++) {
            points[f * RANK_NB + r].rx() = clockwiseRingCoordinates[r][0];
            points[f * RANK_NB + r].ry() = clockwiseRingCoordinates[r][1];
        }
    }
}

void BoardItem::drawBoard(QPainter *painter)
{
#ifndef QT_MOBILE_APP_UI
    QColor shadowColor(128, 42, 42);
    shadowColor.setAlphaF(0.3);
    painter->fillRect(boundingRect(), QBrush(shadowColor));
#endif /* ! QT_MOBILE_APP_UI */

    // Fill in picture
#ifdef QT_MOBILE_APP_UI
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(239, 239, 239));
    painter->drawRect(-boardSideLength / 2, -boardSideLength / 2,
                      boardSideLength, boardSideLength);
#else
    painter->drawPixmap(-boardSideLength / 2, -boardSideLength / 2,
                        boardSideLength, boardSideLength,
                        QPixmap(":/image/resources/image/board.png"));
#endif /* QT_MOBILE_APP_UI */
}

void BoardItem::drawLines(QPainter *painter)
{
    // Solid line brush
#ifdef QT_MOBILE_APP_UI
    QPen pen(QBrush(QColor(241, 156, 159)), LINE_WEIGHT, Qt::SolidLine,
             Qt::SquareCap, Qt::BevelJoin);
#else
    const QPen pen(QBrush(QColor(178, 34, 34)), LINE_WEIGHT, Qt::SolidLine,
                   Qt::SquareCap, Qt::BevelJoin);
#endif
    painter->setPen(pen);

    // No brush
    painter->setBrush(Qt::NoBrush);

    for (uint8_t f = 0; f < FILE_NB; f++) {
        // Draw three boxes
        painter->drawPolygon(f * RANK_NB + points, RANK_NB);
    }

    // Draw 4 vertical and horizontal lines
    for (int r = 0; r < RANK_NB; r += 2) {
        painter->drawLine(points[r], points[(FILE_NB - 1) * RANK_NB + r]);
    }

    if (hasDiagonalLine) {
        // Draw 4 diagonal lines
        for (int r = 1; r < RANK_NB; r += 2) {
            painter->drawLine(points[r], points[(FILE_NB - 1) * RANK_NB + r]);
        }
    }
}

void BoardItem::drawCoordinates(QPainter *painter)
{
    const int FONT_SIZE = 12;

    int offset_x = LINE_WEIGHT + FONT_SIZE / 4;
    int offset_y = LINE_WEIGHT + FONT_SIZE / 4;

    const int extra_offset_x = 4;
    const int extra_offset_y = 1;

    QPen fontPen(QBrush(Qt::darkRed), LINE_WEIGHT, Qt::SolidLine, Qt::SquareCap,
                 Qt::BevelJoin);
    painter->setPen(fontPen);

    QFont font;
    font.setPointSize(FONT_SIZE);
    painter->setFont(font);

    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance("A");

    int origin_x = -boardSideLength / 2 + (boardSideLength / 8) - offset_x;
    int origin_y = boardSideLength / 2 - (boardSideLength / 8) + offset_y;

    int interval = boardSideLength / 8;

    for (int i = 0; i < 7; ++i) {
        QString text = QString(QChar('A' + i));
        painter->drawText(origin_x + interval * i - textWidth / 2 +
                              2 * extra_offset_x,
                          origin_y + 20 + extra_offset_x, text);
    }

    for (int i = 0; i < 7; ++i) {
        QString text = QString::number(i + 1);
        painter->drawText(origin_x - 20 - extra_offset_y,
                          origin_y - interval * i, text);
    }
}

/**
 * @brief Draw polar coordinates on the board.
 *
 * This function sets up the pen and font, and then iteratively draws
 * polar coordinates at specified points on the board. The coordinates
 * are positioned in a manner similar to clock face numbers.
 *
 * @param painter Pointer to the QPainter object for drawing.
 */
void BoardItem::drawPolarCoordinates(QPainter *painter)
{
    QPen fontPen(QBrush(Qt::white), LINE_WEIGHT, Qt::SolidLine, Qt::SquareCap,
                 Qt::BevelJoin);
    painter->setPen(fontPen);
    QFont font;
    font.setPointSize(4);
    font.setFamily("Arial");
    font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
    painter->setFont(font);

    for (int r = 0; r < RANK_NB; r++) {
        QString text('1' + r);
        painter->drawText(points[(FILE_NB - 1) * RANK_NB + r], text);
    }
}

/**
 * @brief Find the point closest to the provided target point among the board's
 * predefined points.
 *
 * This function iterates through an array of predefined points (represented by
 * the member variable 'points'). It returns the point that is closest to the
 * provided target point, based on a set distance threshold (PIECE_SIZE / 2).
 *
 * @param targetPoint The point to which we are finding the closest point from
 * the array 'points'.
 * @return Returns the point closest to targetPoint based on the distance
 * threshold.
 */
QPointF BoardItem::getNearestPoint(const QPointF targetPoint)
{
    // Initialize nearestPoint to the origin (0,0) as a starting point for
    // comparison
    auto nearestPoint = QPointF(0, 0);

    // Iterate through the array of predefined points to find the nearest one to
    // targetPoint
    for (auto pt : points) {
        // Check if the distance between targetPoint and the current point (pt)
        // is within the radius of a piece (PIECE_SIZE / 2)
        if (QLineF(targetPoint, pt).length() < PIECE_SIZE / 2) {
            nearestPoint = pt;
            break;
        }
    }

    return nearestPoint;
}

QPointF BoardItem::polarCoordinateToPoint(File f, Rank r) const
{
    return points[(static_cast<int>(f) - 1) * RANK_NB + static_cast<int>(r) -
                    1];
}

bool BoardItem::pointToPolarCoordinate(QPointF point, File &f, Rank &r) const
{
    // Iterate through all the points to find the closest one to the target
    // point.
    for (int sq = 0; sq < SQUARE_NB; sq++) {
        // If the target point is sufficiently close to one of the predefined
        // points.
        if (QLineF(point, points[sq]).length() < (qreal)PIECE_SIZE / 6) {
            // Calculate the corresponding File and Rank based on the closest
            // point's index.
            f = static_cast<File>(sq / RANK_NB + 1);
            r = static_cast<Rank>(sq % RANK_NB + 1);
            return true;
        }
    }

    return false;
}
