import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.geometry.*;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.*;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.text.*;
import javafx.stage.Stage;
import javafx.animation.*;
import javafx.util.Duration;
import java.io.IOException;
import java.nio.file.*;

public class ScorigamiLiveUI extends Application {

    private Label percentLabel;
    private Label chancePercent;
    private Label subtitle, subtitle2;
    @Override
    public void start(Stage stage) {
        // Root layout
        BorderPane root = new BorderPane();
        root.setStyle("-fx-background-color: #1e1e1e;");

        // ----------- HEADER -----------
        HBox header = new HBox(10);
        header.setPadding(new Insets(15));
        header.setAlignment(Pos.CENTER_LEFT);

        Image ucLogo = new Image(getClass().getResourceAsStream("UC_Logo.png"));
        ImageView logoView = new ImageView(ucLogo);
        logoView.setFitWidth(120);
        logoView.setPreserveRatio(true);

        


        VBox titleBox = new VBox(15);
        titleBox.setTranslateX(5);
        titleBox.setTranslateY(20);
        Label title = new Label("SCORIGAMI TRACKER");
        title.setFont(Font.font("Arial", FontWeight.BOLD, 21));
        title.setTextFill(Color.WHITE);

        subtitle = new Label("UC vs Opponent              Q3 – Loading");
        subtitle.setFont(Font.font("Arial", 14));
        subtitle.setTextFill(Color.LIGHTGRAY);

        subtitle2 = new Label("UC vs Opponent              Q3 – Loading");
        subtitle2.setFont(Font.font("Arial", 14));
        subtitle2.setTextFill(Color.LIGHTGRAY);

        titleBox.getChildren().addAll(title, subtitle, subtitle2);
        header.getChildren().addAll(logoView, titleBox);
        root.setTop(header);

        // ----------- CENTER AREA -----------
        HBox center = new HBox(40);
        center.setPadding(new Insets(20));
        center.setAlignment(Pos.CENTER);

        // Left Circle Placeholder
        StackPane circlePane = new StackPane();
        circlePane.setPrefSize(150, 150);
        Circle ring = new Circle(70);
        ring.setStroke(Color.DARKRED);
        ring.setStrokeWidth(14);
        ring.setFill(Color.TRANSPARENT);

        Label percentLabel = new Label("%");
        percentLabel.setFont(Font.font("Arial", FontWeight.BOLD, 40));
        percentLabel.setTextFill(Color.WHITE);
        Label chancePercent = new Label("%");
        chancePercent.setFont(Font.font("Arial", FontWeight.EXTRA_BOLD, 36));
        chancePercent.setTextFill(Color.WHITE);

        // ----------- PREDICTIONS -----------
        VBox predictionsBox = new VBox(10);
        predictionsBox.setPadding(new Insets(10, 20, 10, 20));
        predictionsBox.setStyle("-fx-background-color: #2a2a2a;");

        Label predictionsTitle = new Label("Top Predicted Final Scores");
        predictionsTitle.setFont(Font.font("Arial", FontWeight.BOLD, 16));
        predictionsTitle.setTextFill(Color.WHITE);

        predictionsBox.getChildren().add(predictionsTitle);

        

         // ----------- FOOTER / HISTORY -----------
        VBox historyBox = new VBox(5);
        historyBox.setPadding(new Insets(10, 20, 20, 20));

        Label historyTitle = new Label("Scorigami History");
        historyTitle.setFont(Font.font("Arial", FontWeight.BOLD, 16));
        historyTitle.setTextFill(Color.WHITE);

        Label hist1 = new Label("Last UC Scorigami: 52–23 vs Miami (2021)");
        Label hist2 = new Label("Most common UC score: 28–24");
        Label hist3 = new Label("Most random UC score: 64–12");

        for (Label lbl : new Label[]{hist1, hist2, hist3}) {
            lbl.setFont(Font.font("Arial", 14));
            lbl.setTextFill(Color.LIGHTGRAY);
        }

        historyBox.getChildren().addAll(historyTitle, hist1, hist2, hist3);
        predictionsBox.getChildren().add(historyBox);

        root.setBottom(predictionsBox);

        Timeline timeline = new Timeline(
            new KeyFrame(Duration.seconds(0), e -> {
                try {
                    java.util.List<String> lines = Files.readAllLines(Paths.get("scorigami_percent.txt"));

                    // Line 1: Game Time
                    if (lines.size() >= 1) {
                        subtitle.setText(lines.get(3).trim() + " @ " + lines.get(2).trim() + "        Q" + lines.get(1).trim() + " - " + lines.get(0).trim());
                        subtitle2.setText(lines.get(5).trim() + " - " + lines.get(4).trim() + "       Q" + lines.get(1).trim() + " - " + lines.get(0).trim());
                    } else {
                        subtitle.setText("UC vs Opponent              N/A");
                    }

                    // Line 2: Scorigami Percentage
                    if (lines.size() >= 2) {
                        String percent = lines.get(6).trim();
                        percentLabel.setText(percent);
                        chancePercent.setText(percent);
                        
                    } else {
                        percentLabel.setText("N/A");
                        chancePercent.setText("N/A");
                    }

                    // Only if there are enough lines
                    if (lines.size() >= 7) {
                        String percent = lines.get(6).trim();
                        percentLabel.setText(percent);
                        chancePercent.setText(percent);

                        // Remove old prediction rows (everything except title and historyBox)
                        predictionsBox.getChildren().removeIf(node -> node != predictionsTitle && node != historyBox);

                    
                        // Use lines 7–11 for the top 5 predictions
                        for (int i = 0; i < 5; i++) {
                            if (lines.size() > 7 + i) {
                                String predictionLine = lines.get(7 + i).trim();

                                HBox row = new HBox();
                                row.setStyle("-fx-background-color: #3a3a3a; -fx-background-radius: 10;");
                                row.setPadding(new Insets(8));
                                row.setAlignment(Pos.CENTER_LEFT);

                                Label predictionLabel = new Label(predictionLine);
                                predictionLabel.setTextFill(Color.WHITE);
                                predictionLabel.setFont(Font.font("Arial", 14));

                                row.getChildren().add(predictionLabel);

                                int insertIndex = predictionsBox.getChildren().indexOf(historyBox);
                                predictionsBox.getChildren().add(insertIndex, row);
                            }
                        }

                    }                    

                    

                } catch (IOException ex) {
                    subtitle.setText("UC vs Opponent              N/A");
                    percentLabel.setText("Error");
                    chancePercent.setText("Error");
                }
            }),
            new KeyFrame(Duration.seconds(3)) // Check every 3 seconds
        );
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();




        Label scoreLabel = new Label("UC 31 – 19");
        scoreLabel.setFont(Font.font("Arial", 14));
        scoreLabel.setTextFill(Color.LIGHTGRAY);
        scoreLabel.setTranslateY(30);

        circlePane.getChildren().addAll(ring, percentLabel, scoreLabel);

        // Right Chance Text
        VBox chanceBox = new VBox(10);
        chanceBox.setAlignment(Pos.CENTER_LEFT);

        Label chanceTitle = new Label("Scorigami Chance");
        chanceTitle.setFont(Font.font("Arial", FontWeight.BOLD, 18));
        chanceTitle.setTextFill(Color.WHITE);

        Label chanceInfo = new Label("Scorigami = a score that\nhas never occurred before\nin college football history!");
        chanceInfo.setFont(Font.font("Arial", 14));
        chanceInfo.setTextFill(Color.LIGHTGRAY);

        chanceBox.getChildren().addAll(chanceTitle, chancePercent, chanceInfo);

        center.getChildren().addAll(circlePane, chanceBox);
        root.setCenter(center);

        
        // ----------- SCENE AND SHOW -----------
        Scene scene = new Scene(root, 400, 700);
        stage.setScene(scene);
        stage.setTitle("Scorigami Tracker");
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}
