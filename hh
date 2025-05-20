import SwiftUI
import CoreLocation
import CoreLocationUI

struct HomeView: View {
    @AppStorage("userName") var userName: String = ""
    @State private var showNameInput = false
    @State private var locationManager = CLLocationManager()
    @State private var currentLocation: CLLocation?
    @State private var locationError = false

    let referenceLongitude = 34.817549168324334

    var body: some View {
        VStack {
            Spacer().frame(height: 40)

            // אפשרות לשינוי שם
            Button("Insert name") {
                showNameInput = true
            }
            .foregroundColor(.blue)
            .underline()

            Spacer().frame(height: 10)

            Text("Hi \(userName.isEmpty ? "Guest" : userName)")
                .font(.title)
                .padding(.bottom, 30)

            HStack {
                VStack {
                    Image("earth_west")
                        .resizable()
                        .frame(width: 100, height: 100)
                    Text("West Side")
                }

                Spacer()

                VStack {
                    Image("earth_east")
                        .resizable()
                        .frame(width: 100, height: 100)
                    Text("East Side")
                }
            }
            .padding(.horizontal, 40)

            Spacer()

            Button("START") {
                requestLocation()
            }
            .padding()
            .foregroundColor(.white)
            .frame(width: 200, height: 50)
            .background(Color.blue)
            .cornerRadius(10)

            if locationError {
                Text("⚠️ המשחק לא יכול לפעול ללא מיקום.")
                    .foregroundColor(.red)
                    .padding()
            }

            Spacer()
        }
        .padding()
        .sheet(isPresented: $showNameInput) {
            NameInputView(name: $userName)
        }
    }

    private func requestLocation() {
        locationManager.requestWhenInUseAuthorization()

        if CLLocationManager.locationServicesEnabled() {
            locationManager.delegate = LocationDelegate { location in
                self.currentLocation = location
                self.locationError = false
                handleLocation(location)
            }
            locationManager.startUpdatingLocation()
        } else {
            locationError = true
        }
    }

    private func handleLocation(_ location: CLLocation) {
        let longitude = location.coordinate.longitude
        if longitude > referenceLongitude {
            print("🔵 East Side")
        } else {
            print("🟢 West Side")
        }
    }
}

// קלט שם פרטי
struct NameInputView: View {
    @Binding var name: String
    @Environment(\.dismiss) var dismiss

    var body: some View {
        VStack(spacing: 20) {
            Text("Enter your name")
                .font(.headline)
            TextField("Name", text: $name)
                .textFieldStyle(RoundedBorderTextFieldStyle())
                .padding()
            Button("Save") {
                dismiss()
            }
        }
        .padding()
    }
}

// האזנה למיקום
class LocationDelegate: NSObject, CLLocationManagerDelegate {
    let onUpdate: (CLLocation) -> Void

    init(onUpdate: @escaping (CLLocation) -> Void) {
        self.onUpdate = onUpdate
    }

    func locationManager(_ manager: CLLocationManager, didUpdateLocations locations: [CLLocation]) {
        manager.stopUpdatingLocation()
        if let location = locations.first {
            onUpdate(location)
        }
    }

    func locationManager(_ manager: CLLocationManager, didFailWithError error: Error) {
        print("Location failed: \(error.localizedDescription)")
    }
}
