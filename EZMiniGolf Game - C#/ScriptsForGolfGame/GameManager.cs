/*
 * File: GameManager.cs
 * Brief: "Main" that brings everything together
 * 
 *          I'll make this my main comment section
 *              I kinda went way over the top with some of this, I just
 *              started and soon I was just overkilling the design and what
 *              I wanted to implement. I spent an inordinate amount of time
 *              on this and there are still issues primarily related to it's
 *              low poly nature and tiny scale. I could upscale but I got it
 *              to behave at small scale with the main major exception of the
 *              hole 6 ramp which likes to rocket the ball off course. I started
 *              messing with MeshLab to fix it and realized I am eating time.
 * 
 *          I know there are better ways of implementing some of these
 *              especially the holeStartPoints, par and holeDetector arrays
 *              that are done by hand in the inspector, but I didn't want
 *              to go out of scope.
 *              
 *          I hope this has stayed in scope. I learned a lot in the process
 *              particularly with the physics engine not implementing things
 *              before a second update frame is run.
 *              
 *          I used kenney minigolf assets from itch.io to make this. The assets
 *              are a combined wall and "ground." This makes it next to impossible
 *              to implement the 1 mark I need for "plays a sound when ball hits
 *              anything" so I left it out. I hope this doesn't mean I'm left out
 *              of the bonus but by the time I realized it there was no going back.
 *              It is what it is.
 * 
 * Author: Peter Gortemaker
 * Date: March 2026
 */

using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public const int totalHoles = 9;
    public int holeIndex = 0;
    public int currentHole = 1;
    public int currentStrokes = 0;
    public int[] strokesPerHole;
    public int difficulty;

    private HolePreview holePreview;

    [Header("References")]
    public Camera mainCamera;
    public UIManager uiManager;
    public AudioManager audioManager;
    public GameObject scorecardPanel;
    public GameObject holePreviewText;
    public WindManager windManager;

    [Header("Hole Setup")]
    public Transform[] holeStartPoints; // for ball drop location
    public HoleDetector[] holeDetectors; // because boost wasn't resetting before holedetector flipped
    public int[] par; // for scorecard
    public Transform ball;
    public Rigidbody ballRb;

    // Physics settings
    [Header("Bouncy Settings")]
    public PhysicsMaterial ballMaterial;
    public PhysicsMaterial courseMaterial;
    public float normalB = 0.5f;
    public float bouncyB = 0.9f;
    public float normalC = 0.3f;
    public float bouncyC = 0.7f;





    void Awake()
    {
        holePreview = GetComponent<HolePreview>(); // for first hole
    }

    void Start()
    {
        // Get and Apply difficulty settings
        difficulty = PlayerPrefs.GetInt("Difficulty", 0);
        ApplyDifficulty();
        if (difficulty == 2 || difficulty == 3)
        {
            windManager.GenerateWind();
            uiManager.UpdateWind(windManager.GetWindForce());
        }

        // Disable scorecard by default, show first hole preview and ball at hole
        if (scorecardPanel) scorecardPanel.SetActive(false);
        holePreview.ShowPreview(holeIndex);
        PlaceBallAtCurrentHole();
    }

    void Update()
    {
        // Disable the holePreview help text
        if (!holePreview.showingPreview)
            holePreviewText.SetActive(false);

        // Pause Menu
        if (Input.GetKey(KeyCode.Escape))
        {
            ShowScorecard();
        }
    }

    void ApplyDifficulty()
    {
        bool bouncy = difficulty == 1 || difficulty == 3;
        bool windy = difficulty == 2 || difficulty == 3;

        // Apply bounciness to ball/course physics material
        if (ballMaterial)
        {
            ballMaterial.bounciness = bouncy ? bouncyB : normalB;
            courseMaterial.bounciness = bouncy ? bouncyC : normalC;
        }

        // Enable wind
        if (windManager)
        {
            windManager.windEnabled = windy;
            windManager.Init(ballRb);
        }
    }





    public void AddStroke()
    {
        currentStrokes++;
        uiManager.UpdateUI(currentHole, currentStrokes);
    }

    // Stops ball movement becuase ball movement was causing multiple triggers
    public void BallInHole()
    {
        audioManager.PlayInHoleSound();

        ballRb.isKinematic = true;
        holeDetectors[holeIndex].ResetTrigger();

        strokesPerHole[holeIndex] = currentStrokes;

        if (currentHole < totalHoles)
        {
            holeIndex++;
            currentHole++;
            currentStrokes = 0;

            uiManager.UpdateUI(currentHole, currentStrokes);

            // Show next hole preview
            holePreviewText.SetActive(true);
            holePreview.ShowPreview(holeIndex);

            // Generate wind for next hole
            windManager.GenerateWind();
            uiManager.UpdateWind(windManager.GetWindForce());

            // Move ball
            PlaceBallAtCurrentHole();
        }
        else
        {
            // End of game
            ShowScorecard();
        }
    }

    void PlaceBallAtCurrentHole()
    {
        if (holeIndex > holeStartPoints.Length) return;

        ball.position = holeStartPoints[holeIndex].position;
        
        ball.gameObject.SetActive(true);

        // Snap the camera to the new hole
        CameraFollow cam = mainCamera.GetComponent<CameraFollow>();
        cam.SnapToTarget();
    }

    void ShowScorecard()
    {
        ball.gameObject.SetActive(false);
        if (scorecardPanel)
        {
            scorecardPanel.SetActive(true);
            scorecardPanel.GetComponent<ScoreCard>().FillScoreCard();
        }
    }

    public int GetTotalStrokes()
    {
        int total = 0;
        foreach (int s in strokesPerHole) total += s;
        return total;
    }


    // For the Score Card Menu
    public void ResumeGame()
    {
        scorecardPanel.SetActive(false);
        Time.timeScale = 1;
        ball.gameObject.SetActive(true);
    }

    public void ResetGame()
    {
        ballRb.linearVelocity = Vector3.zero; // If restarted mid shot, velocity went into the restarted game
        ballRb.angularVelocity = Vector3.zero;
        holeIndex = 0;
        currentHole = 1;
        currentStrokes = 0;

        if (scorecardPanel) scorecardPanel.SetActive(false);
        holePreview.ShowPreview(holeIndex);
        PlaceBallAtCurrentHole();

        uiManager.UpdateUI(currentHole, currentStrokes);
    }

    public void MainMenu()
    {
        SceneManager.LoadScene("MenuScene");
    }

    public void Quit()
    {
        Application.Quit();
    }
}
