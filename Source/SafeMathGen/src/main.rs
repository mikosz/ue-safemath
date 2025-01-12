use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::error::Error;
use tera::{Context, Tera, Value};

#[derive(Deserialize, Serialize)]
struct Unit {
    name: String,
    suffix: String,
}

#[derive(Deserialize, Serialize)]
struct Quantity {
    name: String,
    default_unit_type: String,
    units: Vec<Unit>,
}

#[derive(Deserialize, Serialize)]
struct Params {
    quantities: Vec<Quantity>,
}

// Custom filter to capitalize the first letter of a string
fn capitalize_filter(value: &Value, _: &HashMap<String, Value>) -> tera::Result<Value> {
    // Ensure the value is a string
    let string = value
        .as_str()
        .ok_or_else(|| tera::Error::msg("Expected a string for capitalize_filter"))?;

    let capitalized = capitalize(&string);

    // Return the modified value
    Ok(Value::String(capitalized))
}

fn capitalize(string: &str) -> String {
    let chars = string.chars();
    let mut capitalize_next = true;

    chars.fold(String::new(), |result: String, c: char| {
        if c == '-' {
            capitalize_next = true;
            result
        } else if capitalize_next {
            capitalize_next = false;
            result + c.to_uppercase().collect::<String>().as_str()
        } else {
            result + c.to_string().as_str()
        }
    })
}

fn deserialize_params(params_file: &str) -> Result<Params, Box<dyn Error>> {
    let params_str = std::fs::read_to_string(params_file)?;
    Ok(serde_json::from_str(&params_str)?)
}

fn generate(template_file: &str, items: impl Serialize) -> Result<String, Box<dyn Error>> {
    let template = std::fs::read_to_string(template_file)?;
    let context = Context::from_serialize(&items)?;

    let mut tera = Tera::new("res/**/*")?;

    tera.register_filter("capitalize", capitalize_filter);

    let gen = tera.render_str(&template, &context)?;

    Ok(gen)
}

fn main() {
    let params = deserialize_params("res/quantity-params.json").unwrap();

    for quantity in &params.quantities {
        match generate("res/quantity.tera", &quantity) {
            Ok(gen) => std::fs::write(
                format!(
                    "../SafeMath/Public/SafeMath/Quantity/{}.h",
                    capitalize(&quantity.name)
                ),
                &gen,
            )
            .unwrap(),
            Err(e) => println!("Error while parsing quantity: {}", e),
        }
    }

    match generate("res/register-custom-property-type-layouts.tera", &params) {
        Ok(gen) => std::fs::write(
            "../SafeMathEditor/Private/SafeMath/Quantity/RegisterCustomPropertyTypeLayouts.cpp",
            &gen,
        )
        .unwrap(),
        Err(e) => println!("Error while parsing quantity: {}", e),
    }
}
