#include <stdio.h>

#include "collision.h"
#include "math.h"
#include "brick.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern int GAME_WIDTH;
extern int GAME_HEIGHT;

bool CollisionPointCercle (Point2D A, Ball ball)
{
    Point2D C = ball.position;
    int d2 = (A.x-C.x)*(A.x-C.x) + (A.y-C.y)*(A.y-C.y);
    if (d2>ball.radius*ball.radius)
    {
        return false;
    }
    else 
    {
        return true;
    }
}

bool CollisionDroite (Point2D A, Point2D B, Ball ball)
{
    Vector2D u = Vector(A, B);
    Point2D C = ball.position;
    Vector2D AC = Vector(A, C);
    float numerateur = u.x*AC.y - u.y*AC.x;   // norme du vecteur v
    if (numerateur <0)
        numerateur = -numerateur ;   // valeur absolue ; si c'est négatif, on prend l'opposé.
    float denominateur = sqrt(u.x*u.x + u.y*u.y);  // norme de u
    float CI = numerateur / denominateur;
    
    if (CI<ball.radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CollisionSegment (Point2D A,Point2D B, Ball ball)
{
    Point2D C = ball.position;
    if (CollisionDroite(A,B,ball) == false)
        return false;  // si on ne touche pas la droite, on ne touchera jamais le segment

    Vector2D AB,AC, BC;
    AB = Vector(A, B);
    AC = Vector(A, C);
    BC = Vector(B, C);
    float pscal1 = DotProduct(AB, AC);  // produit scalaire
    float pscal2 = DotProduct(MultVector(AB, -1), BC);  // produit scalaire

    if (pscal1>=0 && pscal2>=0)
    {
        //printf("Collision segment\n");
        return true;   // I entre A et B, ok.
    }
    // dernière possibilité, A ou B dans le cercle
    if (CollisionPointCercle(A,ball))
    {
        //printf("Point A(%.1f,%.1f) dans cercle\n", A.x, A.y);
        return true;
    }
    if (CollisionPointCercle(B,ball))
    {
        //printf("Point B(%.1f,%.1f) dans cercle\n", B.x, B.y);
        return true;
    }
    return false;
}

int BrickCollision (Brick brick, PtBall ptBall)
{
    Point2D A = brickVerticeTopLeft(&brick);
    Point2D B = brickVerticeTopRight(&brick);
    Point2D C = brickVerticeBottomLeft(&brick);
    Point2D D = brickVerticeBottomRight(&brick);

    bool AB, BC, CD, DA;
    AB = CollisionSegment(A, B, *ptBall);
    BC = CollisionSegment(B, C, *ptBall);
    CD = CollisionSegment(C, D, *ptBall);
    DA = CollisionSegment(D, A, *ptBall);

    if (AB == true || BC == true || CD == true || DA == true)
    {
        if (AB == true ) {
            // We put ball outside the brick
            ptBall->position.y = brick.position.y - brick.height/2 - ptBall->radius;
            return 1;
        }
        if (BC == true ) {
            ptBall->position.x = brick.position.x + brick.width/2 + ptBall->radius;
            return 2;
        }
        if (CD == true ) {
            ptBall->position.y = brick.position.y + brick.height/2 + ptBall->radius;
            return 3;
        }
        if (DA == true ) {
            ptBall->position.x = brick.position.x - brick.width/2 - ptBall->radius;
            return 4;
        }
    }    
    else {
        return -1;
    }
    return -1;
}

int collisionBallWall(PtBall ptBall)
{
    float LEFT_BORDER = (WINDOW_WIDTH-GAME_WIDTH)/2;
    float RIGHT_BORDER = GAME_WIDTH + (WINDOW_WIDTH-GAME_WIDTH)/2;

    if (ballLeftPosition(ptBall) <= LEFT_BORDER)
    {
        // To keep ball inside the game
        ptBall->position.x = LEFT_BORDER + ptBall->radius;
        return 1;
    }
    else if (ballRightPosition(ptBall) >= RIGHT_BORDER)
    {
        ptBall->position.x = RIGHT_BORDER - ptBall->radius;
        return 1;
    }
        
    else
        return 0;
}

// Return -1 if inside game, 0 if player 1 lose, 1 if player 2 lose
int ballOutOfGame(PtBall ptBall)
{
    float TOP_BORDER = (WINDOW_HEIGHT-GAME_HEIGHT)/2;
    float BOTTOM_BORDER = GAME_HEIGHT + (WINDOW_HEIGHT-GAME_HEIGHT)/2;

    if (ballTopPosition(ptBall) <= TOP_BORDER)
        return 1;
    else if (ballBottomPosition(ptBall) >= BOTTOM_BORDER)
        return 0;
    else
        return -1;
}

/* Return 1 if collision with bar1, 2 if collision with bar2, 0 if no collision */
int collisionBallBar(PtBall ptBall, PtBar bar1, PtBar bar2)
{
    // Bar 1 collision between the center and the top of the bar
    if (ballBottomPosition(ptBall) >= barTopPosition(bar1) && ballBottomPosition(ptBall) <= bar1->position.y)
    {
        if (ballRightPosition(ptBall) >= barLeftPosition(bar1) && ballLeftPosition(ptBall) <= barRightPosition(bar1))
        {
            ptBall->position.y = bar1->position.y - bar1->height/2 - ptBall->radius;
            return 1;
        }
            
        else return 0;
    }

    // Bar 2 is rotated 180°
    else if (ballTopPosition(ptBall) <= barBottomPosition(bar2) && ballTopPosition(ptBall) >= bar2->position.y)
    {
        if (ballRightPosition(ptBall) >= barLeftPosition(bar2) && ballLeftPosition(ptBall) <= barRightPosition(bar2))
        {
            ptBall->position.y = bar2->position.y + bar2->height/2 + ptBall->radius;
            return 2;
        }
        else return 0;
    }
    else
        return 0;
}

void changeDirection (Vector2D* direction, Orientation orientation)
{
    if (orientation == HORIZONTAL)
    {
        direction->x = inverse(direction->x);
    }
    else if (orientation == VERTICAL)
    {
        direction->y = inverse(direction->y);
    }
}

// Change ball's direction. The closer you are to the bar center, the more vertical it will be
void changeAngle (PtBall ptBall, PtBar ptBar)
{
    float dist;
    float angle;

    dist = ptBall->position.x - ptBar->position.x;
    // To avoid bugs if you move the bar during collision
    if (dist > ptBar->width/2)
        dist = ptBar->width/2;
    else if (dist < inverse(ptBar->width/2))
        dist = inverse(ptBar->width/2);

    // Value between 0 and M_PI/3
    angle = dist/(ptBar->width/2) * M_PI/3;

    ptBall->direction.x = sin(angle);
    if (ptBall->direction.y > 0)
        ptBall->direction.y = -cos(angle);
    else
        ptBall->direction.y = cos(angle);
}

// -1 if ball inside, 0 if player 1 lose, 1 if player 2 lose, 2 if collision detected
int checkPosition (PtBall ptBall, PtBar bar1, PtBar bar2, PtBrick ptBrick)
{
    int colBallBar, colBallBrick;
    int position;

    // 0 if no collision with the bar
    colBallBar = collisionBallBar(ptBall, bar1, bar2);
    colBallBrick = BrickCollision(*ptBrick, ptBall);
    position = ballOutOfGame(ptBall);

    // If ball inside the game
    if (position == -1)
    {
        // If ball hits left or right border
        if (collisionBallWall(ptBall))
        {
            changeDirection(&(ptBall->direction), HORIZONTAL);
            position = 2;
        }

        // If ball hits bar1 (down)
        if (colBallBar == 1)
        {
            changeAngle(ptBall, bar1);
            position = 2;
        }
        // If ball hits bar2 (up)
        else if (colBallBar == 2)
        {
            changeAngle(ptBall, bar2);
            position = 2;
        }

        if (colBallBrick == 2 || colBallBrick == 4)
        {
            changeDirection(&(ptBall->direction), HORIZONTAL);
            position = 2;
        }
        else if (colBallBrick == 1 || colBallBrick == 3)
        {
            changeDirection(&(ptBall->direction), VERTICAL);
            position = 2;
        }
    }
    else
    {
        changeDirection(&(ptBall->direction), VERTICAL);
    }

    return position;
}