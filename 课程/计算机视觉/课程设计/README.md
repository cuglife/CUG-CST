## Steps

1. 修改 第 23 行为自己的 图片目录，如果是自己的数据集，需要相机参数 K.txt，运行

   ```
   python tutorial_demo.py
   ```

2. 转换为 .mvs

   ```
   ./openMVG_main_openMVG2openMVS.exe -i tutorial_out\reconstruction_global\sfm_data.bin -o tutorial_out\reconstruction_global\scene.mvs
   
   ./openMVG_main_openMVG2openMVS.exe -i tutorial_out\reconstruction_sequential\sfm_data.bin -o tutorial_out\reconstruction_sequential\scene.mvs
   ```

3. 将 scene.mvs 和 undistorted_images 拷贝至 MVS 目录下

4. 查看生成的稀疏点云：

   ```
   ./Viewer.exe scene.mvs
   ```

5. 生成稠密点云：

   ```
   ./DensifyPointCloud.exe scene.mvs
   ```


6. 查看稠密点云：

   ```
   ./Viewer.exe scene_dense.mvs
   ```

7. 建立粗网格：

   ```
   ./ReconstructMesh.exe scene_dense.mvs
   ```

8. 查看粗网络

   ```
   ./Viewer.exe scene_dense_mesh.mvs
   ```

9. 生成精细网格（可选），耗时

   ```
   ./RefineMesh.exe scene_dense_mesh.mvs
   ```

10. 查看结果

    ```
    ./Viewer.exe scene_dense_mesh_refine.mvs
    ```

11. 建立混合网格

    ```
    ./TextureMesh.exe scene_dense_mesh_refine.mvs
    ```

12. 查看最终结果

    ```
    ./Viewer.exe scene_dense_mesh_refine_texture.mvs
    ```

    

