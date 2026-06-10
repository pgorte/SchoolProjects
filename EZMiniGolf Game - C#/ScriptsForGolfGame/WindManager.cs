/*
 * File: WindManager.cs
 * Brief: Generates random wind per hole and applies physics to the ball
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class WindManager : MonoBehaviour
{
    public BallController ballControl;

    [Header("Wind Settings")]
    public float minWindForce = 0.5f;
    public float maxWindForce = 2.5f;

    public bool windEnabled = false;

    private Vector3 windForce;
    private Rigidbody ball;

    public Vector3 GetWindForce() => windForce;

    public void Init(Rigidbody rb)
    {
        ball = rb;
        GenerateWind();
    }

    // Change wind per hole 
    public void GenerateWind()
    {
        if (!windEnabled)
        {
            windForce = Vector3.zero;
            return;
        }

        float strength = Random.Range(minWindForce, maxWindForce);
        float angle = Random.Range(0f, 360f);
        Vector3 direction = Quaternion.Euler(0f, angle, 0f) * Vector3.forward;
        windForce = direction * strength;
    }

    // Don't apply at low speed (no infinite rolling)
    void FixedUpdate()
    {
        if (!windEnabled) return;
        if (ball.linearVelocity.magnitude < 0.1f) return;
        ball.AddForce(windForce, ForceMode.Acceleration);
    }
}