/*
 * File: CameraFollow.cs
 * Brief: Main camera function that follows the ball but allows you to control
 *          the view in a wide 360 around the ball with arrow keys
 *          
 *          Also snaps the camera to the ball when moving holes
 *          
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    public Transform ball;
    public float smoothSpeed = 5f;
    public float distance = 6f;
    [Range(10f, 80f)]
    public float pitchAngle = 45f;
    public float rotateSpeed = 2f;

    private float currentYaw = 180f;

    // Arrow key yaw functionality
    void LateUpdate()
    {
        if (Input.GetKey(KeyCode.LeftArrow))
            currentYaw -= rotateSpeed * Time.deltaTime;
        if (Input.GetKey(KeyCode.RightArrow))
            currentYaw += rotateSpeed * Time.deltaTime;

        ApplyCameraPosition(ball.position);
    }

    void ApplyCameraPosition(Vector3 target)
    {
        Quaternion rotation = Quaternion.Euler(pitchAngle, currentYaw, 0f);
        Vector3 offset = rotation * new Vector3(0f, 0f, -distance);
        transform.position = Vector3.Lerp(transform.position, target + offset, smoothSpeed * Time.deltaTime);
        transform.LookAt(target);
    }

    // Snap back to ball after moving to new hole or resetting
    public void SnapToTarget()
    {
        Quaternion rotation = Quaternion.Euler(pitchAngle, currentYaw, 0f);
        Vector3 offset = rotation * new Vector3(0f, 0f, -distance);
        transform.position = ball.position + offset;
        transform.LookAt(ball.position);
    }
}