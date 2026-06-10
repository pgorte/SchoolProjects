/*
 * File: OutOfBounds.cs
 * Brief: Calls TriggerReset() on the ball's BallReset component
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class OutOfBounds : MonoBehaviour
{
    public BallReset ballReset;
    private float cooldown = 0f;

    void Update()
    {
        if (cooldown > 0f)
            cooldown -= Time.deltaTime;
    }

    void OnCollisionEnter(Collision collision)
    {
        if (cooldown > 0f) return;
        if (collision.collider.CompareTag("Ball"))
        {
            cooldown = 3f;
            ballReset.TriggerReset();
        }
    }
}