#include "glimac/scene.hpp"


namespace glimac {

void Scene::sceneInit(GestionCube* ActualGerant){
    for (int y = 0; y < 1; ++y){
        for (int x = 0; x < (SCENE_DIM/2)+1; ++x){
            for (int z = 0; z < SCENE_DIM+1; ++z){
                if (z<=10){
                    ActualGerant->ajoutCube(glm::vec3(-x,y-1,-z), COULEURBASE);
                } else {
                    ActualGerant->ajoutCube(glm::vec3(-x,y-1,z-(SCENE_DIM/2)), COULEURBASE);
                } 
            } 
        }
        for (int x = 1; x < (SCENE_DIM/2)+1; ++x){
            for (int z = 0; z < SCENE_DIM+1; ++z){
                if (z<=(SCENE_DIM/2)){
                    ActualGerant->ajoutCube(glm::vec3(x,y-1,-z), COULEURBASE);
                } else {
                    ActualGerant->ajoutCube(glm::vec3(x,y-1,z-(SCENE_DIM/2)), COULEURBASE);
                } 
            } 
        }
    }
}

void Scene::suppSceneInit(GestionCube* ActualGerant){
    for (int y = 0; y < 1; ++y){
        for (int x = 0; x < (SCENE_DIM/2)+1; ++x){
            for (int z = 0; z < SCENE_DIM+1; ++z){
                if (z<=10){
                    ActualGerant->supprCube(glm::vec3(-x,y-1,-z));
                } else {
                    ActualGerant->supprCube(glm::vec3(-x,y-1,z-(SCENE_DIM/2)));
                } 
            } 
        }
        for (int x = 1; x < (SCENE_DIM/2)+1; ++x){
            for (int z = 0; z < SCENE_DIM+1; ++z){
                if (z<=(SCENE_DIM/2)){
                    ActualGerant->supprCube(glm::vec3(x,y-1,-z));
                } else {
                    ActualGerant->supprCube(glm::vec3(x,y-1,z-(SCENE_DIM/2)));
                } 
            } 
        }
    }
}

void Scene::loadRBFscene(GestionCube* ActualGerant){
     const int nbPoints = 3;
     Eigen::MatrixXd map = Eigen::MatrixXd::Zero(21,21);
     Eigen::MatrixXf ptsDeControle(nbPoints, 2);
     ptsDeControle << 10,10,2,3,15,7;
     Eigen::VectorXf valeurs(nbPoints);
     valeurs << 15,-10,5;

     map = getMap(ptsDeControle, valeurs);

     for (int x = 0; x < 21; x++){
        for (int z = 0; z < 21; z++){
            for (int y = -20; y < map(x,z); y++){
                ActualGerant->ajoutCube(glm::vec3(x-(SCENE_DIM/2),y,z-(SCENE_DIM/2)), COULEURBASE);
            }
        }
     }
}

void Scene::saveScene(GestionCube* ActualGerant){
    savePositions(ActualGerant);
    saveColors(ActualGerant);
}

void Scene::savePositions(GestionCube* ActualGerant){
    //créer un fichier de sauvegarde sceneCube
    std::ofstream objetfichier;
    objetfichier.open("../world_Imaker/sceneCube.txt", std::ios::out); //on ouvrre le fichier en ecriture
    //permet de tester si le fichier s'est ouvert sans probleme 
    if (objetfichier.bad()){
        std::cout<<"échec de la création d'un fichier sauvegarde"<<std::endl;
    } 
    //rentre les données de notre scène dans le fichier de sauvegarde
    for (int i = 0; i < (ActualGerant->getCubesPositionsSize()); i++){
        objetfichier << ActualGerant->getCubesPositions(i).x << " " << ActualGerant->getCubesPositions(i).y << " " << ActualGerant->getCubesPositions(i).z << " ";
    }
    objetfichier.close(); //on ferme le fichier
}

void Scene::saveColors(GestionCube* ActualGerant){
    //créer un fichier de sauvegarde sceneCube
    std::ofstream objetfichier;
    objetfichier.open("../world_Imaker/sceneColors.txt", std::ios::out); //on ouvrre le fichier en ecriture
    //permet de tester si le fichier s'est ouvert sans probleme 
    if (objetfichier.bad()){
        std::cout<<"échec de la création d'un fichier sauvegarde pour les couleurs"<<std::endl;
    } 
    //rentre les données de notre scène dans le fichier de sauvegarde
    for (int i = 0; i < (ActualGerant->getCubesPositionsSize()); i++){
        objetfichier << ActualGerant->getCubesCouleurs(i).x << " " << ActualGerant->getCubesCouleurs(i).y << " " << ActualGerant->getCubesCouleurs(i).z << " ";
    }
    objetfichier.close(); //on ferme le fichier
}

void Scene::chargeScene(GestionCube* ActualGerant){
    //vider mongestionnaire de cube = optionnel
    //accéder à mon fichier de sauvegarde
    std::ifstream objetfichier("../world_Imaker/sceneCube.txt");
    //déclaration d'un entier qui permettra de parcourir le fichier en question
    int lecture;
    std::vector<int> tabPos;
    //permet de tester si le fichier s'est ouvert sans probleme 
    if (objetfichier.bad()){
        std::cout<<"échec de l'ouverture du fichier sauvegarde"<<std::endl;
    } 
    //récupérer ses données et les rentrer dans mon gestionnaire de cube
    //tant qu'on réussi à lire des données on les rentre dans le tableau qui gère nos cube
    while(objetfichier>>lecture){
        tabPos.push_back(lecture);
    }
    objetfichier.close();

    //De meme pour les données de couleurs
    std::ifstream fichierCouleur("../world_Imaker/sceneColors.txt");
    float lectureColors;
    std::vector<float> tabCol;
    if (fichierCouleur.bad()){
        std::cout<<"échec de l'ouverture du fichier sauvegarde"<<std::endl;
    } 
    while(fichierCouleur>>lectureColors){
        tabCol.push_back(lectureColors);
    }
    fichierCouleur.close();
    
    //rentrer données dans nos tableaux de cubes et de couleurs
    for (int i = 0; i < tabPos.size()-2; i=i+3){
        int x =tabPos[i];
        int z =tabPos[i+1];
        int y =tabPos[i+2];
        //ActualGerant->cubesPositions.push_back(glm::vec3(x,z,y));
        //ActualGerant->cubesCouleurs.push_back(glm::vec3(0,0,0));
        ActualGerant->ajoutCube(glm::vec3(x,z,y),glm::vec3(tabCol[i],tabCol[i+1],tabCol[i+2]));
    }
}
}
