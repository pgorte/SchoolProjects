/*
 * File: HoleDetector.cs
 * Brief: Trigger script for knowing when to go to the next hole
 *          This was tricky to implement and is why I have isKinematic calls in gameManager
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class HoleDetector : MonoBehaviour
{
    public GameManager gameManager;

    private bool triggered = false; // prevent hole skipping
    private float timer = 0f;
    public float delay = 2f;

    void OnTriggerEnter(Collider other)
    {
        if (triggered) return;

        if (other.CompareTag("Ball"))
        {
            triggered = true;
            timer = delay;
        }
    }

    // Was having hole skipping issues so I tried all sorts of stuff to get it to work
    void Update()
    {
        if (!triggered) return;

        timer -= Time.deltaTime;

        if (timer <= 0f)
        {
            gameManager.BallInHole();
            triggered = false;
        }
    }

    public void ResetTrigger()
    {
        triggered = false;
    }
}