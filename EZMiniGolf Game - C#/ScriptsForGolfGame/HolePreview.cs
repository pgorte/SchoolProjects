/*
 * File: HolePreview.cs
 * Brief: Switches camera view before each hole so you can see the whole hole
 * Author: Peter Gortemaker
 */

using UnityEngine;

public class HolePreview : MonoBehaviour
{
    [Header("Cameras")]
    public Camera mainCamera;
    public Camera[] holePreviewCameras;

    [Header("AimLine On/Off")]
    public BallController aimLine;

    public GameManager gameManager;

    private int currentHole = 0;
    public bool showingPreview = true;

    void Update()
    {
        if (showingPreview &&
            (Input.GetMouseButtonUp(0)))
            StartHole();
    }

    // Called by GameManager.BallInHole()
    public void ShowPreview(int holeIndex)
    {
        mainCamera.gameObject.SetActive(false);
        aimLine.enabled = false;
        showingPreview = true;

        for (int i = 0; i < holePreviewCameras.Length; i++)
        {
            holePreviewCameras[i].gameObject.SetActive(i == holeIndex);
        }

        currentHole = holeIndex;
    }

    // Switches to main camera ball view and re-activates ball (was having some triggering issues)
    void StartHole()
    {
        aimLine.enabled = true;
        showingPreview = false;

        foreach (Camera cam in holePreviewCameras)
        {
            cam.gameObject.SetActive(false);
        }

        mainCamera.gameObject.SetActive(true);
        gameManager.ballRb.isKinematic = false;
    }
}