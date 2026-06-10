/*
 * File: BallReset.cs
 * Brief: Trigger script for my OutOfBounds planes
 *          - returns to last stopped position on the course
 *          
 *          This was tricky to implement too
 *          Sometimes wouldn't reset, sometimes it would but in the OutOfBounds
 *          Nearly the same reason for the isKinematic calls as HoleDetector
 *          MovePosition saved the game
 *          
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class BallReset : MonoBehaviour
{
    public float resetDelay = 2f;

    private Vector3 lastSafePosition;
    private Rigidbody ball;

    private bool isResetting = false;
    private float resetTimer = float.MaxValue;

    void Start()
    {
        ball = GetComponent<Rigidbody>();
    }

    void Update()
    {
        if (!isResetting) return;
        
        resetTimer -= Time.deltaTime;
        if (resetTimer <= 0f)
            ResetBall();
    }

    public void UpdateSafePosition()
    {
        if (!isResetting)
        {
            lastSafePosition = transform.position;
        }
    }

    public void TriggerReset()
    {
        if (isResetting) return;

        isResetting = true;
        resetTimer = resetDelay;

        ball.isKinematic = true;
    }

    // Had to get the ball to stop moving otherwise it sometimes was misplacing the ball
    void ResetBall()
    {
        Debug.Log($"ResetBall. lastSafePosition={lastSafePosition} current={transform.position}");
        ball.isKinematic = false;

        ball.linearVelocity = Vector3.zero;
        ball.angularVelocity = Vector3.zero;
        ball.MovePosition(lastSafePosition); // KEY to reset working properly

        isResetting = false;
    }
}