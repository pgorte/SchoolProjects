/*
 * File: BallController.cs
 * Brief: Handles ball shooting controls, aim line renderer, main camera follow
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class BallController : MonoBehaviour
{
    [Header("References")]
    public GameManager gameManager;
    public AudioManager audioManager;
    public UIManager uiManager;
    public LineRenderer aimLine;
    public Camera mainCamera;

    [Header("Shot Settings")]
    public float maxForce = 5f;
    public float maxDrag = 3f;
    public float boostMultiplier = 2f; // Boost ball mid roll 2x the current force
    public float stopVelocity = 0.1f; // stop ball movement under this velocity

    public bool canShoot = true;
    private bool isDragging = false;
    private bool canBoost = true;
    private bool hasStartedMoving = false;
    private float lastShotTime;

    private Rigidbody ball;
    private Vector3 mouseDown;

    void Start()
    {
        ball = GetComponent<Rigidbody>();
        if (aimLine) aimLine.enabled = false;
    }

    void Update()
    {
        // Boost once per shot, only while rolling
        if (!canShoot && canBoost && Input.GetKeyDown(KeyCode.Space))
        {
            if (Time.time > lastShotTime + 0.2f && ball.linearVelocity.magnitude > stopVelocity)
                ApplyBoost();
        }

        if (!canShoot)
            RollingState();
        else
            AimingState();
    }

    // Double the speed at the time of spacebar hit
    public void ApplyBoost()
    {
        ball.linearVelocity *= boostMultiplier;
        ball.angularVelocity *= boostMultiplier;
        canBoost = false;
    }

    // Reset for the canShoot and canBoost bools
    // Ball stops under minimum velocity
    public void RollingState()
    {
        if (ball.isKinematic) return;

        if (hasStartedMoving && Time.time > lastShotTime + 0.2f)
        {
            if (ball.linearVelocity.magnitude < stopVelocity && ball.angularVelocity.magnitude < stopVelocity)
            {
                ball.linearVelocity = Vector3.zero;
                ball.angularVelocity = Vector3.zero;

                canShoot = true;
                canBoost = true;
                hasStartedMoving = false;
            }
        }
    }

    // Aim and shoot functionality
    public void AimingState()
    {
        if (Input.GetMouseButtonDown(0))
        {
            // Captured in case we go out of bounds
            GetComponent<BallReset>().UpdateSafePosition();

            // Start aim
            mouseDown = GetMouseWorldPos();
            isDragging = true;
        }

        // Aim
        if (isDragging && Input.GetMouseButton(0))
        {
            Vector3 mouse = GetMouseWorldPos();
            Vector3 drag = mouse - mouseDown;
            drag.y = 0f;

            // Clamp drag to max distance
            float dragDist = Mathf.Min(drag.magnitude, maxDrag);
            Vector3 dragDir = drag.magnitude > 0.01f ? drag.normalized : Vector3.zero;

            // Slingshot: shoot opposite to drag direction
            Vector3 shootDir = -dragDir;
            float power = dragDist / maxDrag;

            if (aimLine)
            {
                aimLine.enabled = true;
                // Line goes from ball toward shoot direction
                aimLine.SetPosition(0, transform.position);
                aimLine.SetPosition(1, transform.position + shootDir * dragDist);

                // Had to look this one up. I wanted it to change based on how far back you pulled
                aimLine.startColor = Color.Lerp(Color.darkGreen, Color.red, power);
                aimLine.endColor = aimLine.startColor;
            }

            // Display power on screen
            uiManager.UpdatePower(power, false);
        }

        // Shoot on MouseUp
        if (isDragging && Input.GetMouseButtonUp(0))
        {
            // Disable line renderer
            isDragging = false;
            if (aimLine) aimLine.enabled = false;

            // Check mouse position at MouseUp
            Vector3 drag = GetMouseWorldPos() - mouseDown;
            drag.y = 0f;

            float dragDist = Mathf.Min(drag.magnitude, maxDrag);
            float power = dragDist / maxDrag;

            // Shoot
            if (power > 0.01f)
            {
                Vector3 force = -drag.normalized * power * maxForce;
                force.y = 0f; // no weirdness

                ball.AddForce(force, ForceMode.Impulse);
                audioManager?.PlayBallHitSound();
                lastShotTime = Time.time;

                canShoot = false;
                canBoost = true;
                hasStartedMoving = true;

                uiManager.UpdatePower(power, true);
                gameManager.AddStroke();
            }
        }
    

        // Raycast mouse onto horizontal plane at ball height
        Vector3 GetMouseWorldPos()
        {
            Ray ray = mainCamera.ScreenPointToRay(Input.mousePosition);
            Plane plane = new Plane(Vector3.up, transform.position);
            if (plane.Raycast(ray, out float dist))
                return ray.GetPoint(dist);
            return transform.position;
        }
    }
}
